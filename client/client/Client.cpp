//
// Created by appad on 23/12/2023.
//

#include "Client.h"
#include "AES.h"
#include "OpenSSL.h"
#include <crypt.h>
#include "CLI11.hpp"
#include "curl/curl.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include <error.h>

// https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/certs
std::string RAW_JWKS = R"({"keys":[{"kid":"9cgFE7e849rMB0fxe2HEud-3noZz-dBPEpdcZHOLOwY","kty":"RSA","alg":"RS256","use":"sig","n":"1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4-LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ_kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc_PSjZl4Ldxk64sz5-c39rOdLN2QW96ypQlp-N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi_RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o_mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHw","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrSBDANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjNaFw0zMzEyMjMxNTExMDNaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4+LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ/kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc/PSjZl4Ldxk64sz5+c39rOdLN2QW96ypQlp+N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi/RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o/mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHwIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQBHda/Ge2ZBMV1bNW2wHd/+PqLzRBmSnvFIO3BdUSgqM1U69+J7WqF/KkvtbcC7MK3OtSv7DNgxswP6nemTDoeG8RY+wkj5QPsXP5waxRB/Hb8TbhVnved4fR35Z3cPIIX+V1A3xgxn2fTl2nSaKIXq1JZHoQ9I3RNh+7zeNi2OhRo2C07f+w66WabowWgSZ6hTiCwfvR2f8KMpuck2Ro42VXMvsK+c0bjY33jXuImtXkim/QCzxQkWG6XNDLmtrv89xA1WhU3plSSXDArXSx7sgwKc2VipCcy1ZPoEMt78ChftAJxVJpb2l+p1vErZy6HAf4OHpfDlSx03cw0IZ5wv"],"x5t":"Namnuyi650xu_T_44vGIWBYbo9Y","x5t#S256":"OZ3egsK_3CObMwS30MdxOGWrXFZSDEjS7fSFO2O2i0Q"},{"kid":"dY_HoGKPr_c6e-G1i2770oV6tfsWorBtRa7cfi5_hs4","kty":"RSA","alg":"RSA-OAEP","use":"enc","n":"vf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm-_ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h-9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU_UA9dqRQi3AMqH_2s-xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF_d_FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA-KdEJZSjT-h9W1DdbUPfP-AU5OxjG9johORT--0txtQ","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrTzzANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjRaFw0zMzEyMjMxNTExMDRaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm+/ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h+9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU/UA9dqRQi3AMqH/2s+xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF/d/FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA+KdEJZSjT+h9W1DdbUPfP+AU5OxjG9johORT++0txtQIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQCWszbfphWqhBhuwZQG9ohycPhXU2fKaYA8+psfgpCbZ0LoK4iPr2D/cph7Anll36j/Dg3Ma9METIHd+7cACEiL0d28kjawZCOBPtSU8fZDH5wKMHuFkv+KxZgP8o1ezQFfJJPbCpIQBSeOM28N3/6cQr5+L8VL4N19lihthXFSHWQGE5lmoIbdKeRVsZChkCIOAUGas9A+GPSWmb6bXAIMFURpCCad7xQJQd6P/ee1ehdWmfy8IriKLAecD724J/388WzUrD3cApGc62RzAzOoCpEQpWZwGoB2+pt3RIGaytfQuK9ssTsqMQhggIJI4z/4fpxNwunSchYVtJOZib9q"],"x5t":"gtgHKuEcS8CGM0jk2-oXSAL3sFU","x5t#S256":"Rt9xuJruGJPxSr4IFnx2Ht1Hj9zP53kT2Z19eCeEMVE"}]})";

Client::Client(int inPort, int outPort, int argc, char **argv) : SocketCommunication(inPort, outPort) {
    this->logger = spdlog::stdout_color_mt("Client");
    App *upload = this->add_subcommand("upload", "Upload a file");
    upload->add_option("-f,--file", this->filename, "Specify file to upload")->required();
    upload->callback([this]() {
        this->upload();
    });


    App *download = this->add_subcommand("download", "Download a file");
    download->add_option("-f,--file", this->filename, "Specify file to download")->required();
    download->callback([this]() {
        this->download();
    });
    App *login = this->add_subcommand("login", "Login to server");
    login->callback([this]() {
        this->login();
    });

    App *list = this->add_subcommand("list", "List files on server");
    App *help = this->add_subcommand("help", "Display help");
    this->start();
    this->test();
    this->parse(argc, argv);
}


void Client::upload(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &base64FileContent) {
    logger->info("Uploading file: {}", filename);
    unsigned int fileContentSize = base64FileContent.length();

    // pad file contents to be multiple of 16
    unsigned int paddedFileContentSize = fileContentSize;
    if (paddedFileContentSize % 16 != 0) {
        paddedFileContentSize += 16 - (paddedFileContentSize % 16);
    }
    auto *paddedFileContent = new unsigned char[paddedFileContentSize];
    memcpy(paddedFileContent, base64FileContent.c_str(), fileContentSize);
    for (unsigned int i = fileContentSize; i < paddedFileContentSize; i++) {
        paddedFileContent[i] = '\0';
    }

    logger->debug("File contents: {}", std::string((char *) paddedFileContent, paddedFileContentSize));
    logger->debug("File contents size: {}", paddedFileContentSize);

    std::string buf(reinterpret_cast<char *>(paddedFileContent));

    std::string env_buf = OpenSSL::aes_encrypt(buf, param.key, param.iv);
    // encrypt file contents
    unsigned char *encryptedFileContent = OpenSSL_Utils::convert_string_to_uchar(env_buf);
    // encode file contents to base64
    std::string encodedFileContent = OpenSSL::base64_encode(
            std::string((char *) encryptedFileContent, paddedFileContentSize));
    logger->debug("Encrypted file contents: {}", encodedFileContent);
    this->send("upload|" + filename_ + "|" + encodedFileContent);
}

std::string Client::download(const std::string &filename_, const OpenSSL_AES_Keys &param) {
    logger->info("Downloading file: {}", filename);
    this->send("download|" + filename_);
    std::string fileContents = this->receiveString();
    logger->debug("File contents: {}", fileContents);

    // decode base64
    std::string decodedFileContent = OpenSSL::base64_decode(fileContents);
    logger->debug("Decoded file contents: {}", decodedFileContent);

    // decrypt file contents
    std::string dec_buf = OpenSSL::aes_decrypt(decodedFileContent, param.key, param.iv);
    logger->debug("Decrypted file contents: {}", dec_buf);
    return dec_buf;
}


void Client::upload() {
    std::cout << "Uploading file: " << this->filename << std::endl;
    OpenSSL_AES_Keys aesKeys;

    aesKeys.key = "p6Ix*(L/6NP)28HZ}_KQ25h@dWD+xB{^";
    aesKeys.iv = "a7fe8fed9f4v8e5d";
    // read file contents
    std::ifstream file(this->filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    auto *fileContent = new unsigned char[size];
    if (file.read((char *) fileContent, size)) {
        logger->debug("File contents: {}", std::string((char *) fileContent, size));
        logger->debug("File contents size: {}", size);
        std::string fileContentString(reinterpret_cast<char *>(fileContent), size);
        std::string base64FileContent = OpenSSL::base64_encode(fileContentString);
        this->upload(this->filename, aesKeys, base64FileContent);
    } else {
        logger->error("Failed to read file");
    }

    file.close();
}

void Client::download() {
    std::cout << "Downloading file: " << this->filename << std::endl;
    OpenSSL_AES_Keys aesKeys;
    aesKeys.key = "p6Ix*(L/6NP)28HZ}_KQ25h@dWD+xB{^";
    aesKeys.iv = "a7fe8fed9f4v8e5d";
    std::string decrypted = this->download(this->filename, aesKeys);
    logger->debug("Decrypted file contents: {}", decrypted);
    // base64 decode
    std::string decoded = OpenSSL::base64_decode(decrypted);
    logger->debug("Decoded file contents: {}", decoded);
    std::ofstream file(this->filename, std::ios::binary);
    file << decoded;
    file.close();
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

void Client::login() {
    spdlog::set_level(spdlog::level::debug);
    // get username and password
    std::string username;
    std::string password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    std::string client_id = "projet-secu";
    std::string client_secret = "cFPXiYvSF1dW0zquutSkMWOROp2cxmt0";
    std::string grant_type = "password";
    std::string scope = "openid";
    // make a curl url-form-encoded request
    std::string url = "https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/token";
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl) {

        username = curl_easy_escape(curl, username.c_str(), (int) username.length());
        password = curl_easy_escape(curl, password.c_str(), (int) password.length());
        client_id = curl_easy_escape(curl, client_id.c_str(), (int) client_id.length());
        client_secret = curl_easy_escape(curl, client_secret.c_str(), (int) client_secret.length());
        grant_type = curl_easy_escape(curl, grant_type.c_str(), (int) grant_type.length());
        scope = curl_easy_escape(curl, scope.c_str(), (int) scope.length());

        std::string postFields =
                "username=" + username + "&password=" + password + "&client_id=" + client_id + "&client_secret=" +
                client_secret + "&grant_type=" + grant_type + "&scope=" + scope;
        std::cout << "Post fields: " << postFields << std::endl;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            logger->error("Curl error: {}", curl_easy_strerror(res));
            return;
        }
        std::cout << "Curl response: " << res << std::endl;
        // parse res into json
        nlohmann::json json = nlohmann::json::parse(readBuffer);
        std::string access_token = json["access_token"];
        std::cout << "Access token: " << access_token << std::endl;
        std::string refresh_token = json["refresh_token"];
        auto decoded = jwt::decode(access_token);
        auto jwks = jwt::parse_jwks(RAW_JWKS);
        auto jwk = jwks.get_jwk(decoded.get_key_id());

        auto issuer = decoded.get_issuer();
        auto x5c = jwk.get_x5c_key_value();

        if (!x5c.empty() && !issuer.empty()) {
            logger->debug("Verifying token...");
            auto verifier =
                    jwt::verify()
                            .allow_algorithm(
                                    jwt::algorithm::rs256(jwt::helper::convert_base64_der_to_pem(x5c), "", "", ""));
            verifier.verify(decoded);

        }


        curl_easy_cleanup(curl);

    }
    std::cout << "Login successful!" << std::endl;
}


