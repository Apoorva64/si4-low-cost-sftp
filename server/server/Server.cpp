//
// Created by appad on 23/12/2023.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Server.h"
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Command/Command.h"

#include "curl_easy.h"
#include "curl_pair.h"
#include "curl_exception.h"
#include "jwt-cpp/jwt.h"
#include "../errors/ServerError.h"
#include "Response/Response.h"

const std::string FILES_FOLDER = "files";

/**
 * @brief Constructs a Server object with specified input and output ports.
 *
 * Initializes the Server object with the input and output ports, sets up a logger,
 * and creates a directory for storing files if it does not exist. It also refreshes
 * the server tokens for Keycloak authentication.
 *
 * @param inPort1 The input port number for the server.
 * @param outPort The output port number for the server.
 */
Server::Server(int inPort1, int outPort) : SocketCommunication(inPort1, outPort) {
    logger = spdlog::stdout_color_mt("Server");
    // create files folder if not exists
    std::filesystem::create_directory(FILES_FOLDER);
    // load secret from env
    if (std::getenv("SERVER_ADMIN_PASSWORD") != nullptr) {
        SERVER_ADMIN_PASSWORD = std::getenv("SERVER_ADMIN_PASSWORD");
    }
    else {
        throw std::runtime_error("SERVER_ADMIN_PASSWORD not set in environment");
    }

    if (std::getenv("KEYCLOAK_CLIENT_SECRET") != nullptr) {
        KEYCLOAK_CLIENT_SECRET = std::getenv("KEYCLOAK_CLIENT_SECRET");
    }
    else {
        throw std::runtime_error("KEYCLOAK_CLIENT_SECRET not set in environment");
    }
    loadJWKS();
    refreshServerTokens();
    App *start = this->add_subcommand("start", "Starts the server");
    start->add_option("-p,--port", this->inPort, "Port to listen on")->required();
    start->add_option("-o,--outport", this->outPort, "Port to send messages to")->required();
    start->add_option("-s,--doSendPort", this->doSendPort, "Whether to send the port number to the server");
    start->add_option("-c,--autoclose", this->autoClose, "Whether to close the connection after one interaction");
    start->callback([&]() {
                        this->start();
                        if (this->doSendPort) {
                            this->send(std::to_string(this->inPort));
                        }
                        this->run();
                    }
    );
    this->logger = spdlog::stdout_color_mt(fmt::format("Server[{},{}]", this->inPort, this->outPort));
    App *reset = this->add_subcommand("reset", "Resets the server (deletes all files and Keycloak resources)");
    reset->callback([&]() {
        logger->info("Resetting server...");
        Reset();
        logger->info("Server reset!");
        std::exit(0);
    });
}


/**
 * @brief Constructs a Server object with a specified input port.
 *
 * Initializes the Server object with the input port and sets up the output port to 0.
 *
 * @param inPort1 The input port number for the server.
 */
Server::Server(int inPort1) : SocketCommunication(inPort1, 0) {

}

/**
 * @brief Handles incoming messages and executes corresponding server actions.
 *
 * Processes incoming messages as Command objects and performs actions based on the command type,
 * such as handling SSL handshake, login, file upload, download, listing, deletion, and token refresh.
 *
 * @param msg The raw string message received from the client.
 */
void Server::handleMessage(const std::string &msg) {
    try {
        if (msg == "Ping") {
            this->send("Pong");
        } else {
            Command command(msg);

            switch (command.commandEnum) {
                case SSL_HANDSHAKE:
                    sslHandshake(command.getArgs());
                    break;
                case LOGIN:
                    login(command.getArgs());
                    break;
                case UPLOAD:
                    uploadFile(command.getArgs());
                    break;
                case DOWNLOAD:
                    downloadFile(command.getArgs());
                    break;
                case LIST:
                    listFiles();
                    break;
                case DELETE:
                    deleteFile(command.getArgs());
                    break;
                case REFRESH_TOKEN:
                    refreshToken(command.getArgs());
                    break;
                case UNKNOWN:
                    logger->error("Unknown command: {}", command.toString());
                    break;
                case INIT_SESSION:
                    logger->error("Invalid command: {}", command.toString());
                    break;
                case SERVER_ERROR:
                    logger->error("Client error: {}", command.toString());
                    break;
            }

            if (this->autoClose && command.commandEnum != SSL_HANDSHAKE && command.commandEnum != LOGIN &&
                command.commandEnum != REFRESH_TOKEN) {
                logger->info("| Server.handleMessage | Closing connection...");
                Server::Close();
                logger->info("Connection closed!");
                std::exit(0);
            }
        }
    }
    catch (PublicServerRuntimeError &e) {
        logger->error("{}", e.what());

        send(Response(ERROR, {e.what()}).toString());

        if (this->autoClose) {
            logger->info("| Server.handleMessage | Closing connection...");
            Server::Close();
            logger->info("| Server.handleMessage | Connection closed!");
            std::exit(0);
        }
    }
    catch (std::exception &e) {
        logger->error("Error: {}", e.what());
        send(Response(ERROR, {"Internal Error: "}).toString());
        if (this->autoClose) {
            logger->info("| Server.handleMessage | Closing connection...");
            Server::Close();
            logger->info("| Server.handleMessage | Connection closed!");
            std::exit(0);
        }
    }
}

/**
 * @brief Deletes a file from the server and its associated Keycloak resource.
 *
 * Validates the user's access token and permissions, then deletes the specified file and
 * its Keycloak resource if the user has the necessary permissions.
 *
 * @param args A vector of strings containing the filename and user's access token.
 */
void Server::deleteFile(std::vector<std::string> args) {
    const std::string &filename = args[0];
    if (OpenSSL::is_base64(filename)) {
        logger->debug("| Server.deleteFile | Filename is base64");
    } else {
        throw PublicServerRuntimeError("Filename is not base64");
    }
    const std::string &user_access_token = args[1];
    auto decoded = jwt::decode(user_access_token);
    // verify token
    try {
        verifier.verify(decoded);
    } catch (std::exception &e) {
        throw PublicServerRuntimeError("Token verification failed: " + std::string(e.what()));
    }
    // check permission
    // open file
    std::ifstream file(std::string(FILES_FOLDER) + "/" + filename, std::ios::binary);
    if (!file.is_open()) {
        throw PublicServerRuntimeError("File not found");
    }
    file.seekg(0, std::ios::end);
    unsigned long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    logger->info("| Server.deleteFile | File size: {}", fileSize);
    std::string fileContentsAndResourceId((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string resourceId = fileContentsAndResourceId.substr(fileContentsAndResourceId.find(SEPARATOR) + 1);
    checkPermissionKeycloak(resourceId, user_access_token, "delete");
    logger->info("| Server.deleteFile | Deleting file: {}", filename);
    deleteKeycloakResource(resourceId);
    std::filesystem::remove(std::string(FILES_FOLDER) + "/" + filename);
    logger->info("| Server.deleteFile | File deleted!");
    this->send(Response(DATA, {"OK"}).toString());

}

/**
 * @brief Lists all files stored on the server.
 *
 * Gathers the names of all files stored in the server's file directory and sends them to the client.
 */
void Server::listFiles() {
    logger->info("| Server.listFiles | Listing files...");
    std::vector<std::string> files;
    for (const auto &entry: std::filesystem::directory_iterator(FILES_FOLDER)) {
        if (OpenSSL::is_base64(entry.path().filename().string())) {
           files.push_back(entry.path().filename().string());
        }
    }
    logger->info("| Server.listFiles | Sending files...");
    send(Response(DATA, files).toString());
    logger->info("| Server.listFiles | Files sent!");
}

/**
 * @brief Handles the file download request from the client.
 *
 * Validates the user's access token and permissions, then sends the requested file's contents to the client.
 *
 * @param args A vector of strings containing the filename and user's access token.
 */
void Server::downloadFile(std::vector<std::string> args) {
    const std::string &user_access_token = args[1];
    std::string base64Filename = args[0];
    // check if filename is base64
    if (OpenSSL::is_base64(base64Filename)) {
        logger->debug("| Server.downloadFile | Filename is base64");
    } else {
        throw PublicServerRuntimeError("Filename is not base64");
    }
    const std::string &filename = OpenSSL::base64_decode(base64Filename);

    logger->info("| Server.downloadFile | Downloading file: {} for token: {}", filename, user_access_token);
    auto decoded = jwt::decode(user_access_token);
    // verify token
    verifier.verify(decoded);
    logger->info("| Server.downloadFile | Downloading file: {}", filename);
    std::ifstream file(std::string(FILES_FOLDER) + "/" + base64Filename, std::ios::binary);
    if (!file.is_open()) {
        throw PublicServerRuntimeError("File not found");
    }
    file.seekg(0, std::ios::end);
    unsigned long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    logger->info("| Server.downloadFile | File size: {}", fileSize);
    std::string fileContentsAndResourceId((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::string fileContents = fileContentsAndResourceId.substr(0, fileContentsAndResourceId.find(SEPARATOR));
    logger->info("| Server.downloadFile | File contents: {}", fileContents);
    std::string resourceId = fileContentsAndResourceId.substr(fileContentsAndResourceId.find(SEPARATOR) + 1);
    checkPermissionKeycloak(resourceId, user_access_token, "download");
    logger->info("| Server.downloadFile | Sending file contents...");
    send(Response(DATA, {fileContents}).toString());
    logger->info("| Server.downloadFile | File sent!");
}

/**
 * @brief Handles the file upload request from the client.
 *
 * Validates the user's access token, creates a Keycloak resource, and stores the file on the server.
 *
 * @param args A vector of strings containing the filename, file contents, and user's access token.
 */
void Server::uploadFile(std::vector<std::string> args) {
    // create keycloak resource
    verifyOrRefreshServerTokens();
    std::string user_access_token = args[2];
    auto decoded = jwt::decode(user_access_token);
    // verify token
    try {
        verifier.verify(decoded);
    } catch (std::exception &e) {
        throw PublicServerRuntimeError("Token verification failed: " + std::string(e.what()));
    }
    std::string sub = decoded.get_subject();
    if (OpenSSL::is_base64(args[0])) {
        logger->debug("| Server.uploadFile | Filename is base64");
    } else {
        throw PublicServerRuntimeError("Filename is not base64");
    }
    if (OpenSSL::is_base64(args[1])) {
        logger->debug("| Server.uploadFile | File is base64");
    } else {
        throw PublicServerRuntimeError("File is not base64");
    }
    std::string filename = OpenSSL::base64_decode(args[0]);
    std::string base64Filename = args[0];
    std::string fileContentsBase64 = args[1];
    logger->info("| Server.uploadFile | Uploading file: {}", filename);

    if (std::filesystem::exists(std::string(FILES_FOLDER) + "/" + base64Filename)) {
        throw PublicServerRuntimeError("File already exists");
    }

    std::ofstream outfile(std::string(FILES_FOLDER) + "/" + base64Filename);
    outfile << fileContentsBase64;
    logger->info("| Server.uploadFile | File uploaded!");
    std::string resourceId;
    try {
        logger->info("| Server.uploadFile | Creating keycloak resource...");
        logger->info("| Server.uploadFile | Owner: {}:{}", sub, filename);
        nlohmann::json json = createKeycloakResource(filename, sub);
        resourceId = json["_id"];
        logger->info("| Server.uploadFile | Keycloak resource created!");
        // append resource id to file
        outfile << SEPARATOR << resourceId;
        outfile.close();
        send(Response(DATA, {"OK"}).toString());
    }
    catch (std::exception &e) {
        outfile.close();
        // delete file
        std::filesystem::remove(std::string(FILES_FOLDER) + "/" + base64Filename);
        throw PrivateServerRuntimeError("Error while creating keycloak resource: " + std::string(e.what()));
    }

    try {
        logger->info("| Server.uploadFile | Adding default permissions...");
        addDefaultPermissionsKeycloak(resourceId, user_access_token);
        logger->info("| Server.uploadFile | Default permissions added!");
    }
    catch (std::exception &e) {
        // delete file
        std::filesystem::remove(std::string(FILES_FOLDER) + "/" + base64Filename);
        deleteKeycloakResource(filename);
        throw PrivateServerRuntimeError("Error while adding default permissions: " + std::string(e.what()));
    }
}

/**
 * @brief Authenticates a user with Keycloak using their username and password.
 *
 * Decodes the base64-encoded username and password, authenticates with Keycloak, and sends the access
 * and refresh tokens back to the client.
 *
 * @param args A vector of strings containing the base64-encoded username and password.
 */
void Server::login(std::vector<std::string> args) {
    std::string username = args[0];
    std::string password = args[1];
    logger->info("| Server.login | Logging in user: {}", username);
    // decode base64
    username = OpenSSL::base64_decode(username);
    password = OpenSSL::base64_decode(password);
    logger->debug("| Server.login | Decoded username: {}", username);

    // login
    try {
        auto decoded = this->login(username, password);
        logger->info("| Server.login | User logged in!");
        std::string access_token = decoded["access_token"];
        logger->debug("| Server.login | Access token: {}", access_token);
        std::string refresh_token = decoded["refresh_token"];
        logger->debug("| Server.login | Refresh token: {}", refresh_token);
        logger->info("| Server.login | Sending tokens...");

        // send OK
        send(Response(DATA, {access_token, refresh_token}).toString());
    } catch (std::exception &e) {
        throw PublicServerRuntimeError("Login failed" + std::string(e.what()));
    }
}

/**
 * @brief Authenticates a user with Keycloak using their username and password.
 *
 * Sends a request to Keycloak to authenticate the user and returns the JSON response containing the tokens.
 *
 * @param username The user's username.
 * @param password The user's password.
 * @return nlohmann::json The JSON response from Keycloak containing the access and refresh tokens.
 */
nlohmann::json Server::login(std::string username, std::string password) {

    // Let's declare a stream
    std::ostringstream stream;

    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>("https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/token");
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    easy.add<CURLOPT_HTTPHEADER>(headers);
    username = curl_easy_escape(easy.get_curl(), username.c_str(), (int) username.length());
    password = curl_easy_escape(easy.get_curl(), password.c_str(), (int) password.length());
    std::string client_id = "projet-secu";
    std::string client_secret = KEYCLOAK_CLIENT_SECRET;
    std::string grant_type = "password";
    std::string scope = "openid";
    client_id = curl_easy_escape(easy.get_curl(), client_id.c_str(), (int) client_id.length());
    client_secret = curl_easy_escape(easy.get_curl(), client_secret.c_str(), (int) client_secret.length());
    grant_type = curl_easy_escape(easy.get_curl(), grant_type.c_str(), (int) grant_type.length());
    scope = curl_easy_escape(easy.get_curl(), scope.c_str(), (int) scope.length());

    std::string postFields =
            "username=" + username + "&password=" + password + "&client_id=" + client_id + "&client_secret=" +
            client_secret + "&grant_type=" + grant_type + "&scope=" + scope;

    easy.add<CURLOPT_POSTFIELDS>(postFields.c_str());
    easy.perform();
    response = stream.str();
    logger->info("| Server.login | Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("| Server.login | JSON: {}", json.dump());
    std::string access_token = json["access_token"];
    logger->debug("| Server.login | Access token: {}", access_token);
    std::string refresh_token = json["refresh_token"];
    logger->debug("| Server.login | Refresh token: {}", refresh_token);
    auto decoded = jwt::decode(access_token);

    auto issuer = decoded.get_issuer();
    auto jwks = jwt::parse_jwks(RAW_JWKS);
    auto jwk = jwks.get_jwk(decoded.get_key_id());
    auto x5c = jwk.get_x5c_key_value();
    this->verifier =
            this->verifier
                    .allow_algorithm(
                            jwt::algorithm::rs256(jwt::helper::convert_base64_der_to_pem(x5c), "", "", "")).leeway(100);
    if (!issuer.empty()) {
        logger->debug("| Server.login | Verifying token...");
        verifier.verify(decoded);
        logger->info("| Server.login | Token verified!");
        return json;
    }
    throw PublicServerRuntimeError("Token verification failed");
}

/**
 * @brief Refreshes the server's access token with Keycloak.
 *
 * Logs in the server admin with Keycloak to refresh the server's access token.
 */
void Server::refreshServerTokens() {
    logger->info("| Server.refreshServerTokens | Refreshing server tokens...");
    auto decoded = this->login("admin", SERVER_ADMIN_PASSWORD);
    this->resourceServerAccessToken = decoded["access_token"];
}

/**
 * @brief Verifies or refreshes the server's access token.
 *
 * Verifies the server's current access token and refreshes it if necessary.
 */
void Server::verifyOrRefreshServerTokens() {
    logger->info("| Server.verifyOrRefreshServerTokens | Verifying server tokens...");
    auto decoded = jwt::decode(this->resourceServerAccessToken);
    auto issuer = decoded.get_issuer();
    if (!issuer.empty()) {
        logger->debug("| Server.verifyOrRefreshServerTokens | Verifying token...");
        try {
            verifier.verify(decoded);
        }
        catch (std::exception &e) {
            logger->warn("| Server.verifyOrRefreshServerTokens | Token verification failed: {}", e.what());
            refreshServerTokens();
            return;
        }
    }
    refreshServerTokens();
}

/**
 * @brief Creates a Keycloak resource for a file.
 *
 * Sends a request to Keycloak to create a resource for the file with the specified filename and owner.
 *
 * @param filename The name of the file.
 * @param owner The owner of the file.
 * @return nlohmann::basic_json<> The JSON response from Keycloak containing the resource details.
 */
nlohmann::basic_json<> Server::createKeycloakResource(std::string filename, const std::string &owner) const{
    logger->info("| Server.createKeycloakResource | Creating keycloak resource for file: {}", filename);
    std::string createResourceUrl = "https://keycloak.auth.apoorva64.com/admin/realms/projet-secu/clients/1aa674a2-3169-4041-bc82-dbe6cf1de68c/authz/resource-server/resource";
    // Let's declare a stream
    std::ostringstream stream;

    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>(createResourceUrl.c_str());
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // set access token
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->resourceServerAccessToken).c_str());
    easy.add<CURLOPT_HTTPHEADER>(headers);
    auto j = nlohmann::json();
    j["name"] = filename;
    j["owner"] = owner;
    j["ownerManagedAccess"] = true;
    j["scopes"] = nlohmann::json::array(
            {"delete", "download"}
    );


    std::string postFields = j.dump();
    try {
        easy.add<CURLOPT_POSTFIELDS>(postFields.c_str());
        easy.perform();
    } catch (curl::curl_easy_exception &error) {
        throw PublicServerRuntimeError("Error while performing request: " + std::string(error.what()));
    }

    response = stream.str();
    logger->info("| Server.createKeycloakResource | Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("| Server.createKeycloakResource | JSON: {}", json.dump());
    return json;
}


/**
 * @brief Deletes a Keycloak resource associated with a file.
 *
 * Sends a request to Keycloak to delete the resource associated with the specified filename.
 *
 * @param filename The name of the file whose Keycloak resource should be deleted.
 */
void Server::deleteKeycloakResource(const std::string &filename) const{
    logger->info("| Server.deleteKeycloakResource | Deleting keycloak resource for file: {}", filename);
    std::string deleteResourceUrl =
            "https://keycloak.auth.apoorva64.com/admin/realms/projet-secu/clients/1aa674a2-3169-4041-bc82-dbe6cf1de68c/authz/resource-server/resource/" +
            filename;
    // Let's declare a stream
    std::ostringstream stream;


    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>(deleteResourceUrl.c_str());
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // set access token
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->resourceServerAccessToken).c_str());
    easy.add<CURLOPT_HTTPHEADER>(headers);
    try {
        easy.add<CURLOPT_CUSTOMREQUEST>("DELETE");
        easy.perform();
        logger->info("| Server.deleteKeycloakResource | Keycloak resource deleted! for file: {}", filename);
    } catch (curl::curl_easy_exception &error) {
        throw PrivateServerRuntimeError("Error while performing request: " + std::string(error.what()));
    }

}

/**
 * @brief Checks if the user has the specified permission for a file in Keycloak.
 *
 * Sends a permission check request to Keycloak for the specified file, user access token, and permission.
 *
 * @param filename The name of the file.
 * @param requesterAccessToken The access token of the user requesting permission.
 * @param permission The permission to check (e.g., "download" or "delete").
 */
void
Server::checkPermissionKeycloak(std::string filename, const std::string &requesterAccessToken, std::string permission) {
    auto decoded = jwt::decode(requesterAccessToken);
    logger->info("| Server.checkPermissionKeycloak | Checking permission: {} for file: {} for user: {}", permission, filename, decoded.get_subject());
    // verify token
    verifier.verify(decoded);
    std::string checkPermissionUrl = "https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/token";
    // Let's declare a stream
    std::ostringstream stream;

    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>(checkPermissionUrl.c_str());
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    // set access token
    headers = curl_slist_append(headers, ("Authorization: Bearer " + requesterAccessToken).c_str());
    easy.add<CURLOPT_HTTPHEADER>(headers);

    std::string grant_type = "urn:ietf:params:oauth:grant-type:uma-ticket";
    std::string audience = "projet-secu";
    std::string permission2 = filename + "#" + permission;
    std::string response_mode = "decision";
    std::string submit_request = "true";
    grant_type = curl_easy_escape(easy.get_curl(), grant_type.c_str(), (int) grant_type.length());
    audience = curl_easy_escape(easy.get_curl(), audience.c_str(), (int) audience.length());
    permission2 = curl_easy_escape(easy.get_curl(), permission2.c_str(), (int) permission2.length());
    response_mode = curl_easy_escape(easy.get_curl(), response_mode.c_str(), (int) response_mode.length());
    submit_request = curl_easy_escape(easy.get_curl(), submit_request.c_str(), (int) submit_request.length());

    std::string postFields =
            "grant_type=" + grant_type + "&audience=" + audience + "&permission=" + permission2 + "&response_mode=" +
            response_mode + "&submit_request=" + submit_request;

    easy.add<CURLOPT_POSTFIELDS>(postFields.c_str());
    easy.perform();
    response = stream.str();
    logger->info("| Server.checkPermissionKeycloak | Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    // check if result is a key
    if (json.find("result") != json.end()) {
        bool result = json["result"];
        if (result) {
            logger->info("| Server.checkPermissionKeycloak | Permission granted for file: {} for user: {}", filename, decoded.get_subject());
            return;
        }

    }
    throw PublicServerRuntimeError("Permission denied");
}

/**
 * @brief Refreshes a user's access token with Keycloak using their refresh token.
 *
 * Sends a request to Keycloak to refresh the user's access token using their refresh token and returns the JSON response.
 *
 * @param refresh_token The refresh token to use for refreshing the access token.
 * @return nlohmann::json The JSON response from Keycloak containing the new access and refresh tokens.
 */
nlohmann::json Server::refreshToken(std::string refresh_token) {
    // Let's declare a stream
    std::ostringstream stream;

    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>("https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/token");
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    easy.add<CURLOPT_HTTPHEADER>(headers);
    refresh_token = curl_easy_escape(easy.get_curl(), refresh_token.c_str(), (int) refresh_token.length());
    std::string client_id = "projet-secu";

    std::string postFields =
            "refresh_token=" + refresh_token + "&client_id=" + client_id + "&grant_type=refresh_token";

    easy.add<CURLOPT_POSTFIELDS>(postFields.c_str());
    easy.perform();
    response = stream.str();
    logger->info("| Server.refreshToken | Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("| Server.refreshToken | JSON: {}", json.dump());
    std::string access_token = json["access_token"];
    logger->debug("| Server.refreshToken | Access token: {}", access_token);
    std::string refresh_token2 = json["refresh_token"];

    auto decoded = jwt::decode(access_token);
    verifier.verify(decoded);
    return json;
}

/**
 * @brief Handles a token refresh request from the client.
 *
 * Refreshes the client's access token using the provided refresh token and sends the new tokens back to the client.
 *
 * @param args A vector of strings containing the refresh token.
 */
void Server::refreshToken(std::vector<std::string> args) {
    std::string refresh_token = args[0];
    logger->info("| Server.refreshToken | Refreshing token: {}", refresh_token);
    try {
        auto jsonResponse = this->refreshToken(refresh_token);
        logger->info("| Server.refreshToken | Token refreshed!");
        // send OK
        send(Response(DATA, {jsonResponse["access_token"], jsonResponse["refresh_token"]}).toString());
    } catch (std::exception &e) {
        throw PrivateServerRuntimeError("Refresh failed: " + std::string(e.what()));
    }
}

/**
 * @brief Adds default permissions for a Keycloak resource.
 *
 * Sends a request to Keycloak to add default permissions for the specified resource and owner access token.
 *
 * @param resourceId The ID of the Keycloak resource.
 * @param ownerAccessToken The access token of the owner of the resource.
 */
void Server::addDefaultPermissionsKeycloak(std::string resourceId, const std::string &ownerAccessToken) {
    logger->info("| Server.addDefaultPermissionsKeycloak | Adding default permissions for file: {}", resourceId);
    std::string permissionUrl =
            "https://keycloak.auth.apoorva64.com/realms/projet-secu/account/resources/" + resourceId + "/permissions";
    // get preffered_username from access token
    auto decoded = jwt::decode(ownerAccessToken);
    std::string username = nlohmann::json::parse(decoded.get_payload())["preferred_username"];
    logger->info("| Server.addDefaultPermissionsKeycloak | Owner: {}", username);

    // Let's declare a stream
    std::ostringstream stream;

    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>(permissionUrl.c_str());
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // set access token
    headers = curl_slist_append(headers, ("Authorization: Bearer " + ownerAccessToken).c_str());
    easy.add<CURLOPT_HTTPHEADER>(headers);
    nlohmann::json owner = nlohmann::json();
    owner["username"] = username;
    owner["scopes"] = nlohmann::json::array(
            {"delete", "download"}
    );

    nlohmann::json j = nlohmann::json::array({owner});
    // PUT request
    std::string postFields = j.dump();
    try {
        easy.add<CURLOPT_CUSTOMREQUEST>("PUT");
        easy.add<CURLOPT_POSTFIELDS>(postFields.c_str());
        easy.perform();
    } catch (curl::curl_easy_exception &error) {
        throw PrivateServerRuntimeError("Error while performing request: " + std::string(error.what()));
    }

    logger->info("| Server.addDefaultPermissionsKeycloak | Default permissions added!");
}

/**
 * @brief Handles the SSL handshake process with the client.
 *
 * Performs the SSL handshake by exchanging public keys with the client, decrypting the AES key and IV,
 * and completing the challenge-response verification.
 *
 * @param args A vector of strings containing the client's public key.
 */
void Server::sslHandshake(std::vector<std::string> args) {
    if (args.size() != 1) {
        throw PrivateServerRuntimeError("Error args SSL Handshake");
    }
    logger->info("| Server.sslHandshake | Init SSL Handshake");
    std::string pubKey = OpenSSL::base64_decode(args.at(0));

    this->keyClient = OpenSSL_Utils::get_key_from_str(pubKey, "");
    this->keyServer = OpenSSL::rsa_key_generation();

    if (this->keyClient == nullptr) {
        throw PrivateServerRuntimeError("key client null");
    }

    logger->info("| Server.sslHandshake | KeyGeneration Complete");

    std::string pubServerKey = OpenSSL::base64_encode(OpenSSL_Utils::get_rsa_public_key_str(this->keyServer));

    logger->info("| Server.sslHandshake | Send key");
    this->send(pubServerKey);

    std::string encKey = OpenSSL::base64_decode(this->receiveString());
    std::string encIv = OpenSSL::base64_decode(this->receiveString());

    std::string aesKey = OpenSSL::rsa_decrypt(this->keyServer, encKey);
    std::string aesIv = OpenSSL::rsa_decrypt(this->keyServer, encIv);

    this->key = OpenSSL_Utils::get_aes_key_from_str(aesKey, aesIv);

    std::string encChallenge = OpenSSL::base64_decode(this->receiveString());

    std::string challenge = OpenSSL::aes_decrypt(encChallenge, aesKey, aesIv);

    std::reverse(challenge.begin(), challenge.end());

    encChallenge = OpenSSL::aes_encrypt(challenge, aesKey, aesIv);

    this->send(OpenSSL::base64_encode(encChallenge));

    this->isSslNegotiate = true;
    logger->info("| Server.sslHandshake | SSL Handshake complete !");
}

/**
 * @brief Resets all Keycloak resources.
 *
 * Retrieves and deletes all Keycloak resources associated with the server's client in Keycloak.
 */
void Server::ResetKeycloak() {
    logger->info("| Server.resetKeycloak | Reset Keycloak");

    // Get all resources
    std::string deleteResourceUrl =
            "https://keycloak.auth.apoorva64.com/admin/realms/projet-secu/clients/1aa674a2-3169-4041-bc82-dbe6cf1de68c/authz/resource-server/resource?first=0&max=11000000&deep=false";
    // Let's declare a stream
    std::ostringstream stream;

    // We are going to put the request's output in the previously declared stream
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>(deleteResourceUrl.c_str());
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // set access token
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->resourceServerAccessToken).c_str());
    easy.add<CURLOPT_HTTPHEADER>(headers);
    try {
        easy.add<CURLOPT_CUSTOMREQUEST>("GET");
        easy.perform();
        logger->info("| Server.resetKeycloak | List of resources");
    } catch (curl::curl_easy_exception &error) {
        throw PrivateServerRuntimeError("Error while performing request" + std::string(error.what()));
    }

    response = stream.str();
    logger->info("| Server.resetKeycloak | Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("| Server.resetKeycloak | JSON: {}", json.dump());
    for (auto &element: json) {
        std::string resourceId = element["_id"];
        logger->info("| Server.resetKeycloak | Deleting resource: {}", resourceId);
        deleteKeycloakResource(resourceId);
    }
}

void Server::Reset() {
    logger->info("| Server.reset | Reset Server");
    ResetKeycloak();
    for (const auto &entry: std::filesystem::directory_iterator(FILES_FOLDER)) {
        std::filesystem::remove(entry.path());
    }
}


void Server::loadJWKS() {
    logger->info("| Server.reset | Loading JWKS from Keycloak");
    // Let's declare a stream
    std::ostringstream stream;
    curl::curl_ios<std::ostringstream> ios(stream);
    curl::curl_easy easy(ios);
    easy.add<CURLOPT_URL>("https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/certs");
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);
    std::string response;

    // set Content-Type
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    easy.add<CURLOPT_HTTPHEADER>(headers);
    try {
        easy.add<CURLOPT_CUSTOMREQUEST>("GET");
        easy.perform();
        logger->info("| Server.reset | Got JWKS from Keycloak {}", response);
        this->RAW_JWKS = stream.str();
    } catch (curl::curl_easy_exception &error) {
        throw PrivateServerRuntimeError("Error while performing request" + std::string(error.what()));
    }
}