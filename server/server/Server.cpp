//
// Created by appad on 23/12/2023.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <error.h>
#include "Server.h"
#include "OpenSSL.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Command/CommandEnum.h"
#include "Command/Command.h"

#include "curl_easy.h"
#include "curl_pair.h"
#include "curl_form.h"
#include "curl_exception.h"
#include "../../cmake-build-debug/_deps/nlohmann_json-src/include/nlohmann/json.hpp"
#include "jwt-cpp/jwt.h"

#define SPERATOR '|'
#define FILES_FOLDER "files"
//https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/certs
std::string RAW_JWKS = R"({"keys":[{"kid":"9cgFE7e849rMB0fxe2HEud-3noZz-dBPEpdcZHOLOwY","kty":"RSA","alg":"RS256","use":"sig","n":"1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4-LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ_kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc_PSjZl4Ldxk64sz5-c39rOdLN2QW96ypQlp-N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi_RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o_mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHw","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrSBDANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjNaFw0zMzEyMjMxNTExMDNaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4+LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ/kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc/PSjZl4Ldxk64sz5+c39rOdLN2QW96ypQlp+N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi/RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o/mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHwIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQBHda/Ge2ZBMV1bNW2wHd/+PqLzRBmSnvFIO3BdUSgqM1U69+J7WqF/KkvtbcC7MK3OtSv7DNgxswP6nemTDoeG8RY+wkj5QPsXP5waxRB/Hb8TbhVnved4fR35Z3cPIIX+V1A3xgxn2fTl2nSaKIXq1JZHoQ9I3RNh+7zeNi2OhRo2C07f+w66WabowWgSZ6hTiCwfvR2f8KMpuck2Ro42VXMvsK+c0bjY33jXuImtXkim/QCzxQkWG6XNDLmtrv89xA1WhU3plSSXDArXSx7sgwKc2VipCcy1ZPoEMt78ChftAJxVJpb2l+p1vErZy6HAf4OHpfDlSx03cw0IZ5wv"],"x5t":"Namnuyi650xu_T_44vGIWBYbo9Y","x5t#S256":"OZ3egsK_3CObMwS30MdxOGWrXFZSDEjS7fSFO2O2i0Q"},{"kid":"dY_HoGKPr_c6e-G1i2770oV6tfsWorBtRa7cfi5_hs4","kty":"RSA","alg":"RSA-OAEP","use":"enc","n":"vf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm-_ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h-9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU_UA9dqRQi3AMqH_2s-xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF_d_FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA-KdEJZSjT-h9W1DdbUPfP-AU5OxjG9johORT--0txtQ","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrTzzANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjRaFw0zMzEyMjMxNTExMDRaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm+/ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h+9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU/UA9dqRQi3AMqH/2s+xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF/d/FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA+KdEJZSjT+h9W1DdbUPfP+AU5OxjG9johORT++0txtQIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQCWszbfphWqhBhuwZQG9ohycPhXU2fKaYA8+psfgpCbZ0LoK4iPr2D/cph7Anll36j/Dg3Ma9METIHd+7cACEiL0d28kjawZCOBPtSU8fZDH5wKMHuFkv+KxZgP8o1ezQFfJJPbCpIQBSeOM28N3/6cQr5+L8VL4N19lihthXFSHWQGE5lmoIbdKeRVsZChkCIOAUGas9A+GPSWmb6bXAIMFURpCCad7xQJQd6P/ee1ehdWmfy8IriKLAecD724J/388WzUrD3cApGc62RzAzOoCpEQpWZwGoB2+pt3RIGaytfQuK9ssTsqMQhggIJI4z/4fpxNwunSchYVtJOZib9q"],"x5t":"gtgHKuEcS8CGM0jk2-oXSAL3sFU","x5t#S256":"Rt9xuJruGJPxSr4IFnx2Ht1Hj9zP53kT2Z19eCeEMVE"}]})";
std::string RESSOURCE_SERVER = "https://keycloak.auth.apoorva64.com/admin/realms/projet-secu";

Server::Server(int inPort1, int outPort) : SocketCommunication(inPort1, outPort) {
    this->logger = spdlog::stdout_color_mt("Server");
    // create files folder if not exists
    std::filesystem::create_directory(FILES_FOLDER);
    refreshServerTokens();

}

Server::Server(int inPort1) : SocketCommunication(inPort1, 0) {

}

void Server::handleMessage(const std::string &msg) {
    SocketCommunication::handleMessage(msg);
    Command command(msg);

    switch (command.commandEnum) {
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

void Server::deleteFile(std::vector<std::string> args) {
    const std::string &filename = args[0];
    logger->info("Deleting file: {}", filename);
    std::filesystem::remove(std::string(FILES_FOLDER) + "/" + filename);
    logger->info("File deleted!");
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
    const std::string &filename = args[0];
    const std::string &user_access_token = args[1];
    auto decoded = jwt::decode(user_access_token);
    // verify token
    verifier.verify(decoded);
    std::string sub = decoded.get_subject();
    checkPermissionKeycloak(filename, sub, "download");
    logger->info("Downloading file: {}", filename);
    std::ifstream file(std::string(FILES_FOLDER) + "/" + filename, std::ios::binary);
    if (!file.is_open()) {
        logger->error("File not found");
    }
    file.seekg(0, std::ios::end);
    unsigned long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    logger->info("File size: {}", fileSize);
    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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
    verifier.verify(decoded);
    std::string sub = decoded.get_subject();
    std::string filenameStr = args[0];
    std::filesystem::path path(filenameStr);
    std::string filename = path.filename();
    std::string file = args[1];
    logger->info("Uploading file: {}", filename);
    std::ofstream outfile(std::string(FILES_FOLDER) + "/" + filename);
    outfile << file;
    outfile.close();
    logger->info("File uploaded!");
    logger->info("Creating keycloak resource...");
    logger->info("Owner: {}:{}", sub, filename);
    createKeycloakResource(filename, sub);
    logger->info("Keycloak resource created!");
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
        send("ERROR");
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
            jwt::verify()
                    .allow_algorithm(
                            jwt::algorithm::rs256(jwt::helper::convert_base64_der_to_pem(x5c), "", "", ""));
    if (!issuer.empty()) {
        logger->debug("Verifying token...");
        verifier.verify(decoded);
        return json;
    }
    throw std::runtime_error("Token verification failed");
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
        verifier.verify(decoded);
        return;
    }
    refreshServerTokens();
}

void Server::createKeycloakResource(std::string filename, const std::string &owner) {
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
        send("ERROR");
        return;
    }

    response = stream.str();
    logger->info("Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("JSON: {}", json.dump());
}


void Server::checkPermissionKeycloak(std::string filename, const std::string& owner, std::string permission) {
    logger->info("Checking permission: {} for file: {}", permission, filename);
    std::string checkPermissionUrl = "https://keycloak.auth.apoorva64.com/admin/realms/projet-secu/clients/1aa674a2-3169-4041-bc82-dbe6cf1de68c/authz/resource-server/resource";
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
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // set access token
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->resourceServerAccessToken).c_str());
    easy.add<CURLOPT_HTTPHEADER>(headers);
    nlohmann::json j = nlohmann::json();
    j["name"] = filename;
    j["owner"] = owner;
    j["ownerManagedAccess"] = true;
    j["scopes"] = nlohmann::json::array(
            {permission}
    );

    std::string postFields = j.dump();
    try {
        easy.add<CURLOPT_POSTFIELDS>(postFields.c_str());
        easy.perform();
    } catch (curl::curl_easy_exception &error) {
        logger->error("Error while performing request: {}", error.what());
        send("ERROR");
        return;
    }

    response = stream.str();
    logger->info("Response: {}", response);
    // parse response
    nlohmann::json json = nlohmann::json::parse(response);
    logger->debug("JSON: {}", json.dump());
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
        send("ERROR");
    }
}

