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

#define SPERATOR '|'
#define FILES_FOLDER "files"
//https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/certs
std::string RAW_JWKS = R"({"keys":[{"kid":"9cgFE7e849rMB0fxe2HEud-3noZz-dBPEpdcZHOLOwY","kty":"RSA","alg":"RS256","use":"sig","n":"1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4-LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ_kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc_PSjZl4Ldxk64sz5-c39rOdLN2QW96ypQlp-N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi_RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o_mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHw","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrSBDANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjNaFw0zMzEyMjMxNTExMDNaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4+LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ/kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc/PSjZl4Ldxk64sz5+c39rOdLN2QW96ypQlp+N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi/RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o/mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHwIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQBHda/Ge2ZBMV1bNW2wHd/+PqLzRBmSnvFIO3BdUSgqM1U69+J7WqF/KkvtbcC7MK3OtSv7DNgxswP6nemTDoeG8RY+wkj5QPsXP5waxRB/Hb8TbhVnved4fR35Z3cPIIX+V1A3xgxn2fTl2nSaKIXq1JZHoQ9I3RNh+7zeNi2OhRo2C07f+w66WabowWgSZ6hTiCwfvR2f8KMpuck2Ro42VXMvsK+c0bjY33jXuImtXkim/QCzxQkWG6XNDLmtrv89xA1WhU3plSSXDArXSx7sgwKc2VipCcy1ZPoEMt78ChftAJxVJpb2l+p1vErZy6HAf4OHpfDlSx03cw0IZ5wv"],"x5t":"Namnuyi650xu_T_44vGIWBYbo9Y","x5t#S256":"OZ3egsK_3CObMwS30MdxOGWrXFZSDEjS7fSFO2O2i0Q"},{"kid":"dY_HoGKPr_c6e-G1i2770oV6tfsWorBtRa7cfi5_hs4","kty":"RSA","alg":"RSA-OAEP","use":"enc","n":"vf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm-_ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h-9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU_UA9dqRQi3AMqH_2s-xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF_d_FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA-KdEJZSjT-h9W1DdbUPfP-AU5OxjG9johORT--0txtQ","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrTzzANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjRaFw0zMzEyMjMxNTExMDRaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm+/ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h+9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU/UA9dqRQi3AMqH/2s+xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF/d/FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA+KdEJZSjT+h9W1DdbUPfP+AU5OxjG9johORT++0txtQIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQCWszbfphWqhBhuwZQG9ohycPhXU2fKaYA8+psfgpCbZ0LoK4iPr2D/cph7Anll36j/Dg3Ma9METIHd+7cACEiL0d28kjawZCOBPtSU8fZDH5wKMHuFkv+KxZgP8o1ezQFfJJPbCpIQBSeOM28N3/6cQr5+L8VL4N19lihthXFSHWQGE5lmoIbdKeRVsZChkCIOAUGas9A+GPSWmb6bXAIMFURpCCad7xQJQd6P/ee1ehdWmfy8IriKLAecD724J/388WzUrD3cApGc62RzAzOoCpEQpWZwGoB2+pt3RIGaytfQuK9ssTsqMQhggIJI4z/4fpxNwunSchYVtJOZib9q"],"x5t":"gtgHKuEcS8CGM0jk2-oXSAL3sFU","x5t#S256":"Rt9xuJruGJPxSr4IFnx2Ht1Hj9zP53kT2Z19eCeEMVE"}]})";

Server::Server(int inPort1, int outPort) : SocketCommunication(inPort1, outPort) {
    this->logger = spdlog::stdout_color_mt(fmt::format("Server[{},{}]", inPort1, outPort));
    // create files folder if not exists
    std::filesystem::create_directory(FILES_FOLDER);
    refreshServerTokens();

}

Server::Server(int inPort1) : SocketCommunication(inPort1, 0) {

}

void Server::handleMessage(const std::string &msg) {
    try {
        SocketCommunication::handleMessage(msg);
        Command command(msg);

        switch (command.commandEnum) {
            case SSL_HANDSHAKE:
                sslHandshake(command.args);
                break;
            case LOGIN:
                login(command.args);
                break;
            case UPLOAD:
                uploadFile(command.args);
                break;
            case DOWNLOAD:
                downloadFile(command.args);
                break;
            case LIST:
                listFiles();
                break;
            case DELETE:
                deleteFile(command.args);
                break;
            case REFRESH_TOKEN:
                refreshToken(command.args);
                break;
            case UNKNOWN:
                logger->error("Unknown command: {}", command.toString());
                break;
        }
    }
    catch (std::exception &e) {
        logger->error("Error: {}", e.what());
        send("ERROR");
    }

}

void Server::deleteFile(std::vector<std::string> args) {
    const std::string &filename = args[0];
    if (OpenSSL::is_base64(filename)) {
        logger->debug("Filename is base64");
    } else {
        logger->debug("Filename is not base64");
        throw std::runtime_error("Filename is not base64");
    }
    const std::string &user_access_token = args[1];
    auto decoded = jwt::decode(user_access_token);
    // verify token
    try {
        verifier.verify(decoded);
    } catch (std::exception &e) {
        logger->error("Token verification failed: {}", e.what());
        throw std::runtime_error("Token verification failed");
    }
    // check permission
    // open file
    std::ifstream file(std::string(FILES_FOLDER) + "/" + filename, std::ios::binary);
    if (!file.is_open()) {
        logger->error("File not found");
        throw std::runtime_error("File not found");
    }
    file.seekg(0, std::ios::end);
    unsigned long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    logger->info("File size: {}", fileSize);
    std::string fileContentsAndResourceId((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string resourceId = fileContentsAndResourceId.substr(fileContentsAndResourceId.find(SPERATOR) + 1);
    checkPermissionKeycloak(resourceId, user_access_token, "delete");
    logger->info("Deleting file: {}", filename);
    deleteKeycloakResource(resourceId);
    std::filesystem::remove(std::string(FILES_FOLDER) + "/" + filename);
    logger->info("File deleted!");
    this->send("OK");

}

void Server::listFiles() {
    logger->info("Listing files...");
    std::string files;
    for (const auto &entry: std::filesystem::directory_iterator(FILES_FOLDER)) {
        files += entry.path().filename().string() + SPERATOR;
    }
    logger->info("Sending files...");
    send(files);
    logger->info("Files sent!");
}

void Server::downloadFile(std::vector<std::string> args) {
    const std::string &user_access_token = args[1];
    std::string base64Filename = args[0];
    // check if filename is base64
    if (OpenSSL::is_base64(base64Filename)) {
        logger->debug("Filename is base64");
    } else {
        logger->debug("Filename is not base64");
        throw std::runtime_error("Filename is not base64");
    }
    const std::string &filename = OpenSSL::base64_decode(base64Filename);

    logger->info("Downloading file: {} for token: {}", filename, user_access_token);
    auto decoded = jwt::decode(user_access_token);
    // verify token
    verifier.verify(decoded);
    logger->info("Downloading file: {}", filename);
    std::ifstream file(std::string(FILES_FOLDER) + "/" + base64Filename, std::ios::binary);
    if (!file.is_open()) {
        logger->error("File not found");
        throw std::runtime_error("File not found");
    }
    file.seekg(0, std::ios::end);
    unsigned long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    logger->info("File size: {}", fileSize);
    std::string fileContentsAndResourceId((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::string fileContents = fileContentsAndResourceId.substr(0, fileContentsAndResourceId.find(SPERATOR));
    logger->info("File contents: {}", fileContents);
    std::string resourceId = fileContentsAndResourceId.substr(fileContentsAndResourceId.find(SPERATOR) + 1);
    checkPermissionKeycloak(resourceId, user_access_token, "download");
    logger->info("Sending file contents...");
    send(fileContents);
    logger->info("File sent!");
}

void Server::uploadFile(std::vector<std::string> args) {
    // create keycloak resource
    verifyOrRefreshServerTokens();
    std::string user_access_token = args[2];
    auto decoded = jwt::decode(user_access_token);
    // verify token
    try {
        verifier.verify(decoded);
    } catch (std::exception &e) {
        logger->error("Token verification failed: {}", e.what());
        throw std::runtime_error("Token verification failed");
    }
    std::string sub = decoded.get_subject();
    if (OpenSSL::is_base64(args[0])) {
        logger->debug("Filename is base64");
    } else {
        logger->debug("Filename is not base64");
        throw std::runtime_error("Filename is not base64");
    }
    if (OpenSSL::is_base64(args[1])) {
        logger->debug("File is base64");
    } else {
        logger->debug("File is not base64");
        throw std::runtime_error("File is not base64");
    }
    std::string filename = OpenSSL::base64_decode(args[0]);
    std::string base64Filename = args[0];
    std::string fileContentsBase64 = args[1];
    logger->info("Uploading file: {}", filename);
    std::ofstream outfile(std::string(FILES_FOLDER) + "/" + base64Filename);
    outfile << fileContentsBase64;
    logger->info("File uploaded!");
    std::string resourceId;
    try {
        logger->info("Creating keycloak resource...");
        logger->info("Owner: {}:{}", sub, filename);
        nlohmann::json json = createKeycloakResource(filename, sub);
         resourceId = json["_id"];
        logger->info("Keycloak resource created!");
        // append resource id to file
        outfile << SPERATOR << resourceId;
        outfile.close();
        send("OK");
    }
    catch (std::exception &e) {
        logger->error("Error: {}", e.what());
        outfile.close();
        // delete file
        std::filesystem::remove(std::string(FILES_FOLDER) + "/" + base64Filename);
        throw std::runtime_error("Error while creating keycloak resource");
    }

    try {
        logger->info("Adding default permissions...");
        addDefaultPermissionsKeycloak(resourceId, user_access_token);
        logger->info("Default permissions added!");
    }
    catch (std::exception &e) {
        logger->error("Error: {}", e.what());
        // delete file
        std::filesystem::remove(std::string(FILES_FOLDER) + "/" + base64Filename);
        deleteKeycloakResource(filename);
        throw std::runtime_error("Error while adding default permissions");
    }


}

void Server::login(std::vector<std::string> args) {
    std::string username = args[0];
    std::string password = args[1];
    logger->info("Logging in user: {}", username);
    // decode base64
    username = OpenSSL::base64_decode(username);
    password = OpenSSL::base64_decode(password);
    logger->debug("Decoded username: {}", username);

    // login
    try {
        auto decoded = this->login(username, password);
        logger->info("User logged in!");
        std::string access_token = decoded["access_token"];
        logger->debug("Access token: {}", access_token);
        std::string refresh_token = decoded["refresh_token"];
        logger->debug("Refresh token: {}", refresh_token);
        logger->info("Sending tokens...");

        // send OK
        send(access_token + SPERATOR + refresh_token);
    } catch (std::exception &e) {
        logger->error("Login failed: {}", e.what());
        throw std::runtime_error("Login failed");
    }
}


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
    std::string client_secret = "cFPXiYvSF1dW0zquutSkMWOROp2cxmt0";
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
    logger->info("Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("JSON: {}", json.dump());
    std::string access_token = json["access_token"];
    logger->debug("Access token: {}", access_token);
    std::string refresh_token = json["refresh_token"];
    logger->debug("Refresh token: {}", refresh_token);
    auto decoded = jwt::decode(access_token);

    auto issuer = decoded.get_issuer();
    auto jwks = jwt::parse_jwks(RAW_JWKS);
    auto jwk = jwks.get_jwk(decoded.get_key_id());
    auto x5c = jwk.get_x5c_key_value();
    this->verifier =
            this->verifier
                    .allow_algorithm(
                            jwt::algorithm::rs256(jwt::helper::convert_base64_der_to_pem(x5c), "", "", ""));
    if (!issuer.empty()) {
        logger->debug("Verifying token...");
        //verifier.verify(decoded); #TODO UNCOMMENT THIS LINE OF CODE
        logger->info("Token verified!");
        return json;
    }
    throw std::runtime_error("Token verification failed: ");
}

void Server::refreshServerTokens() {
    logger->info("Refreshing server tokens...");
    auto decoded = this->login("admin", "ZKqudE5ZDxUA7xf");
    this->resourceServerAccessToken = decoded["access_token"];
}

void Server::verifyOrRefreshServerTokens() {
    logger->info("Verifying server tokens...");
    auto decoded = jwt::decode(this->resourceServerAccessToken);
    auto issuer = decoded.get_issuer();
    if (!issuer.empty()) {
        logger->debug("Verifying token...");
        try {
            verifier.verify(decoded);
        }
        catch (std::exception &e) {
            logger->warn("Token verification failed: {}", e.what());
            refreshServerTokens();
            return;
        }
    }
    refreshServerTokens();
}

nlohmann::basic_json<> Server::createKeycloakResource(std::string filename, const std::string &owner) {
    logger->info("Creating keycloak resource for file: {}", filename);
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
    nlohmann::json j = nlohmann::json();
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
        logger->error("Error while performing request: {}", error.what());
        throw std::runtime_error("Error while performing request");
    }

    response = stream.str();
    logger->info("Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("JSON: {}", json.dump());
    return json;
}


void Server::deleteKeycloakResource(const std::string &filename) {
    logger->info("Deleting keycloak resource for file: {}", filename);
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
        std::cout << stream.str() << std::endl;
        logger->info("Keycloak resource deleted! for file: {}", filename);
    } catch (curl::curl_easy_exception &error) {
        logger->error("Error while performing request: {}", error.what());
        throw std::runtime_error("Error while performing request");
    }

}

void
Server::checkPermissionKeycloak(std::string filename, const std::string &requesterAccessToken, std::string permission) {
    auto decoded = jwt::decode(requesterAccessToken);
    logger->info("Checking permission: {} for file: {} for user: {}", permission, filename, decoded.get_subject());
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
    logger->info("Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    // check if result is a key
    if (json.find("result") != json.end()) {
        bool result = json["result"];
        if (result) {
            logger->info("Permission granted for file: {} for user: {}", filename, decoded.get_subject());
            return;
        }

    }
    logger->error("Permission denied!");
    throw std::runtime_error("Permission denied");
}


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
    logger->info("Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("JSON: {}", json.dump());
    std::string access_token = json["access_token"];
    logger->debug("Access token: {}", access_token);
    std::string refresh_token2 = json["refresh_token"];

    auto decoded = jwt::decode(access_token);
    verifier.verify(decoded);
    return json;
}


void Server::refreshToken(std::vector<std::string> args) {
    std::string refresh_token = args[0];
    logger->info("Refreshing token: {}", refresh_token);
    try {
        auto jsonResponse = this->refreshToken(refresh_token);
        logger->info("Token refreshed!");
        // send OK
        send(jsonResponse["access_token"].dump() + SPERATOR + jsonResponse["refresh_token"].dump());
    } catch (std::exception &e) {
        logger->error("Refresh failed: {}", e.what());
        throw std::runtime_error("Refresh failed");
    }
}



void Server::addDefaultPermissionsKeycloak(std::string resourceId, const std::string &ownerAccessToken) {
    logger->info("Adding default permissions for file: {}", resourceId);
    std::string permissionUrl = "https://keycloak.auth.apoorva64.com/realms/projet-secu/account/resources/" + resourceId + "/permissions";
    // get preffered_username from access token
    auto decoded = jwt::decode(ownerAccessToken);
    std::string username = nlohmann::json::parse(decoded.get_payload())["preferred_username"];
    logger->info("Owner: {}", username);

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
        logger->error("Error while performing request: {}", error.what());
        throw std::runtime_error("Error while performing request");
    }

    logger->info("Default permissions added!");
}

void Server::sslHandshake(std::vector<std::string> args) {
    if(args.size() != 1){
        throw std::runtime_error("Error args SSL Handshake");
    }
    logger->info("Init SSL Handshake");
    std::string pubKey = OpenSSL::base64_decode(args.at(0));

    this->keyClient = OpenSSL_Utils::get_key_from_str(pubKey, "");
    this->keyServer = OpenSSL::rsa_key_generation();

    if(this->keyClient == nullptr){
        throw std::runtime_error("key client null");
    }

    logger->info("KeyGeneration Complete");

    std::string pubServerKey = OpenSSL::base64_encode(OpenSSL_Utils::get_rsa_public_key_str(this->keyServer));

    logger->info("Send key");
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
    logger->info("SSL Handshake complete !");
}
