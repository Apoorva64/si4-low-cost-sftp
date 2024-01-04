//
// Created by appad on 23/12/2023.
//

#include "Client.h"
#include "OpenSSL.h"
#include "CLI11.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "Command/Command.h"
#include <jwt-cpp/jwt.h>
#include <error.h>

extern "C" {
#include "libclient.h"
#include "libserver.h"
}

// https://keycloak.auth.apoorva64.com/realms/projet-secu/protocol/openid-connect/certs
std::string RAW_JWKS = R"({"keys":[{"kid":"9cgFE7e849rMB0fxe2HEud-3noZz-dBPEpdcZHOLOwY","kty":"RSA","alg":"RS256","use":"sig","n":"1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4-LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ_kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc_PSjZl4Ldxk64sz5-c39rOdLN2QW96ypQlp-N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi_RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o_mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHw","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrSBDANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjNaFw0zMzEyMjMxNTExMDNaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1f8yK8W9dOu2GXvA4pZAxVQLeKkLiU5UsQs0Eyux64yqjMiO9hhRXlwaLLH5aG4wqGRmcFUYBS4+LrkTzyTTrNPIurcLTm5qRhMb0ZGyv0uYZQvxRHvGjg6ZmGrv9KNlBcwJoVRAZ/kvXRlBGrDYgCkqpx7yzgsAPDf9Aqc/PSjZl4Ldxk64sz5+c39rOdLN2QW96ypQlp+N6hlINmwcFXiRcMhsA47TCKmlRIhfqFup3aaN8ishUyIbemrHZi/RsdJubq8Ddf32PIkaGxS2UMQOLMBEhUbjoCob0Vd0C00o/mi4eGR9eAe070lVkLBF9Z6IfTT5J5QC9l0soPNlHwIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQBHda/Ge2ZBMV1bNW2wHd/+PqLzRBmSnvFIO3BdUSgqM1U69+J7WqF/KkvtbcC7MK3OtSv7DNgxswP6nemTDoeG8RY+wkj5QPsXP5waxRB/Hb8TbhVnved4fR35Z3cPIIX+V1A3xgxn2fTl2nSaKIXq1JZHoQ9I3RNh+7zeNi2OhRo2C07f+w66WabowWgSZ6hTiCwfvR2f8KMpuck2Ro42VXMvsK+c0bjY33jXuImtXkim/QCzxQkWG6XNDLmtrv89xA1WhU3plSSXDArXSx7sgwKc2VipCcy1ZPoEMt78ChftAJxVJpb2l+p1vErZy6HAf4OHpfDlSx03cw0IZ5wv"],"x5t":"Namnuyi650xu_T_44vGIWBYbo9Y","x5t#S256":"OZ3egsK_3CObMwS30MdxOGWrXFZSDEjS7fSFO2O2i0Q"},{"kid":"dY_HoGKPr_c6e-G1i2770oV6tfsWorBtRa7cfi5_hs4","kty":"RSA","alg":"RSA-OAEP","use":"enc","n":"vf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm-_ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h-9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU_UA9dqRQi3AMqH_2s-xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF_d_FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA-KdEJZSjT-h9W1DdbUPfP-AU5OxjG9johORT--0txtQ","e":"AQAB","x5c":["MIICpTCCAY0CBgGMlzrTzzANBgkqhkiG9w0BAQsFADAWMRQwEgYDVQQDDAtwcm9qZXQtc2VjdTAeFw0yMzEyMjMxNTA5MjRaFw0zMzEyMjMxNTExMDRaMBYxFDASBgNVBAMMC3Byb2pldC1zZWN1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvf5jtEYbBHr9gkW7NJBdFDinwLXXC1TgGDKjWmlxthRESfQhk6Sm+/ij22RbcNYtnpidXW7vF0OdmSi0EGupkog7CuRuLOfsQ8h+9Fvh6BehMPYRw1ICro74rESD5gspHAadgI4gnWl9QcSH6EFlck2L796KrPbtBwIecqAJVBK6uP8MfVjcRhU/UA9dqRQi3AMqH/2s+xy1yWyqEPIimjY7wOYM7d9t5Gz4a6KsZYePF/d/FW7K0m4K7rD93wQKgbZVTscdSNvcL1NmeZ8TWV7KlNDHqLdh2h5Cdfrp9oA+KdEJZSjT+h9W1DdbUPfP+AU5OxjG9johORT++0txtQIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQCWszbfphWqhBhuwZQG9ohycPhXU2fKaYA8+psfgpCbZ0LoK4iPr2D/cph7Anll36j/Dg3Ma9METIHd+7cACEiL0d28kjawZCOBPtSU8fZDH5wKMHuFkv+KxZgP8o1ezQFfJJPbCpIQBSeOM28N3/6cQr5+L8VL4N19lihthXFSHWQGE5lmoIbdKeRVsZChkCIOAUGas9A+GPSWmb6bXAIMFURpCCad7xQJQd6P/ee1ehdWmfy8IriKLAecD724J/388WzUrD3cApGc62RzAzOoCpEQpWZwGoB2+pt3RIGaytfQuK9ssTsqMQhggIJI4z/4fpxNwunSchYVtJOZib9q"],"x5t":"gtgHKuEcS8CGM0jk2-oXSAL3sFU","x5t#S256":"Rt9xuJruGJPxSr4IFnx2Ht1Hj9zP53kT2Z19eCeEMVE"}]})";

/**
 * @brief Construct a new Client object.
 *
 * This constructor initializes a new Client object. It sets up the logger,
 * defines the subcommands for the CLI application, and starts the client.
 * It also negotiates the SSL handshake with the server and parses the command-line arguments.
 *
 * @param inPort The input port number.
 * @param outPort The output port number.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 */
Client::Client(int inPort, int outPort, int argc, char **argv) : SocketCommunication(inPort, outPort) {
    this->logger = spdlog::stdout_color_mt("Client");
    this->add_option("-i,--inPort", this->inPort, "Specify input port number");
    App *upload = this->add_subcommand("upload", "Upload a file");
    upload->add_option("-f,--file", this->filename, "Specify file to upload")->required()->check(CLI::ExistingFile);
    upload->callback([this]() {
        this->start();
        this->upload();
    });


    App *download = this->add_subcommand("download", "Download a file");
    download->add_option("-f,--file", this->filename, "Specify file to download")->required();
    download->callback([this]() {
        this->start();
        this->download();
    });
    App *login = this->add_subcommand("login", "Login to server");
    login->callback([this]() {
        this->start();
        this->login();
    });
    App *deleteFile = this->add_subcommand("delete", "Delete a file");
    deleteFile->add_option("-f,--file", this->filename, "Specify file to delete")->required();
    deleteFile->callback([this]() {
        this->start();
        this->deleteFile(this->filename);
    });

    App *list = this->add_subcommand("list", "List files on server");
    list->callback([this]() {
        this->start();
        this->listFiles();
    });
    App *help = this->add_subcommand("help", "Display help");
    this->parse(argc, argv);
    this->logger->info("Input port: {}", this->inPort);
    this->start();
    this->test();
    this->negotiate();
}

/**
 * @brief Start the client.
 *
 * This function starts the client by calling the start function of the SocketCommunication class.
 * It then sends an INIT_SESSION command to the server with the input port number.
 * After receiving a response from the server, it sends an OK message back to the server with the new output port number.
 * If the server responds with an OK message, the function logs that the session initialization is successful.
 * It also sets the output port number to the new output port number received from the server.
 * If there is an error in receiving a message from the server, it throws a runtime error.
 */
void Client::start() {
    startserver(8081);
    Command cmd(INIT_SESSION, {std::to_string(this->inPort)});
    SocketCommunication::send(cmd.toString());
    stopserver();
    SocketCommunication::start();
    char *msg2 = new char[1024];
    int error = getmsg(msg2);
    if (error == -1) {
        logger->error("LibServer error");
        throw std::runtime_error("LibServer error");
    }
    std::string newOutPort(msg2);
    std::string ok = "OK";
    char *msg = new char[1024];
    strcpy(msg, ok.c_str());
    sndmsg(msg, std::stoi(newOutPort));
    error = getmsg(msg2);
    if (error == -1) {
        logger->error("LibServer error");
        throw std::runtime_error("LibServer error");
    }
    logger->info("Init session OK");
    this->logger->info("New Port {}", newOutPort);
    this->outPort = std::stoi(newOutPort);

}

/**
 * @brief Negotiates SSL handshake with the server.
 *
 * This method initiates the SSL handshake process with the server by generating an RSA key pair,
 * sending the public key to the server, and receiving the server's public key. It then generates
 * an AES key and IV, encrypts them with the server's public key, and sends them to the server.
 * A challenge is sent to the server to ensure the integrity of the communication. If the server
 * responds correctly to the challenge, the SSL handshake is considered successful.
 *
 * @throws std::runtime_error If the challenge fails or there is an error during the handshake.
 */
void Client::negotiate() {
    this->logger->info("Start SSL negotiate");
    this->keyServer = OpenSSL::rsa_key_generation();

    std::string serverPubKey = OpenSSL::base64_encode(OpenSSL_Utils::get_rsa_public_key_str(this->keyServer));

    Command cmd(SSL_HANDSHAKE, {serverPubKey});

    this->logger->info("Send Key");
    this->send(cmd.toString());

    std::string serverClientKey = OpenSSL::base64_decode(this->receiveString());


    this->keyClient = OpenSSL_Utils::get_key_from_str(serverClientKey, "");

    this->key = OpenSSL::aes_key_generation();

    this->logger->info("Send Crypt AES");
    std::string aesKey = OpenSSL::base64_encode(OpenSSL::rsa_encrypt(this->keyClient, this->key->key));
    std::string aesIv = OpenSSL::base64_encode(OpenSSL::rsa_encrypt(this->keyClient, this->key->iv));

    this->send(aesKey);
    this->send(aesIv);

    this->logger->info("Start challenge");
    std::string challenge = OpenSSL_Utils::generateRandomString(256);
    std::string encChallenge = OpenSSL::base64_encode(OpenSSL::aes_encrypt(challenge, this->key->key, this->key->iv));

    this->send(encChallenge);

    std::string rawChallenge = OpenSSL::base64_decode(this->receiveString());
    std::string tryChallenge = OpenSSL::aes_decrypt(rawChallenge, this->key->key, this->key->iv);

    std::reverse(challenge.begin(), challenge.end());
    if (challenge != tryChallenge) {
        throw std::runtime_error("Error Challenge Failed !");
    }

    this->logger->info("Challenge OK!");

    this->isSslNegotiate = true;
    this->logger->info("SSL Handshake complete !");
}

/**
 * @brief Uploads a file to the server.
 *
 * This method uploads a file to the server by encrypting the file content with AES, encoding it
 * in base64, and then sending it along with the filename and access token to the server.
 * It checks the server's response to confirm if the upload was successful.
 *
 * @param filename_ The name of the file to upload.
 * @param param The AES keys used for encryption.
 * @param base64FileContent The base64-encoded content of the file.
 * @param accessToken The access token for authentication.
 * @throws std::runtime_error If the upload fails.
 */
void Client::upload(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &base64FileContent,
                    const std::string &accessToken) {
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
    std::string base64FileName = OpenSSL::base64_encode(filename_);
    Command command(UPLOAD, {base64FileName, encodedFileContent, accessToken});
    this->send(command.toString());
    std::string response = this->receiveString();
    logger->debug("Response: {}", response);
    if (response.substr(0, 2) != "OK") {
        std::cout << "Upload failed!" << std::endl;
        throw std::runtime_error("Upload failed");
    }
    std::cout << "Upload successful!" << std::endl;
}

/**
 * @brief Downloads a file from the server.
 *
 * This method requests a file from the server by sending the filename and access token.
 * It receives the file content, decodes it from base64, and decrypts it using AES.
 *
 * @param filename_ The name of the file to download.
 * @param param The AES keys used for decryption.
 * @param accessToken The access token for authentication.
 * @return std::string The decrypted content of the file.
 * @throws std::runtime_error If the download fails.
 */
std::string
Client::download(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &accessToken) {
    logger->info("Downloading file: {}", filename);
    std::string base64FileName = OpenSSL::base64_encode(filename_);
    Command command(DOWNLOAD, {base64FileName, accessToken});
    this->send(command.toString());
    std::string fileContents = this->receiveString();
    if (fileContents == "ERROR") {
        std::cout << "Download failed!" << std::endl;
        throw std::runtime_error("Download failed");
    }
    logger->debug("File contents: {}", fileContents);

    // decode base64
    std::string decodedFileContent = OpenSSL::base64_decode(fileContents);
    logger->debug("Decoded file contents: {}", decodedFileContent);

    // decrypt file contents
    std::string dec_buf = OpenSSL::aes_decrypt(decodedFileContent, param.key, param.iv);
    logger->debug("Decrypted file contents: {}", dec_buf);
    return dec_buf;
}

/**
 * @brief Deletes a file from the server.
 *
 * This method sends a request to the server to delete a file specified by the filename.
 * It checks the server's response to confirm if the deletion was successful.
 *
 * @param filename_ The name of the file to delete.
 * @throws std::runtime_error If the deletion fails.
 */
void Client::deleteFile(std::string filename_) {
    this->RefreshIfNeededOrLogin();
    logger->info("Deleting file: {}", filename_);
    std::string base64FileName = OpenSSL::base64_encode(filename_);
    Command command(DELETE, {base64FileName, this->accessToken});
    this->send(command.toString());
    std::string response = this->receiveString();
    logger->debug("Response: {}", response);
    if (response.substr(0, 2) != "OK") {
        std::cout << "Delete failed!" << std::endl;
        throw std::runtime_error("Delete failed");
    }
    std::cout << "Delete successful!" << std::endl;
}

/**
 * @brief Handles the file upload process.
 *
 * This method reads the content of the file specified by the filename, encrypts it,
 * encodes it in base64, and calls the upload method to send it to the server.
 */
void Client::upload() {
    this->RefreshIfNeededOrLogin();
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
        this->RefreshIfNeededOrLogin();
        this->upload(this->filename, aesKeys, base64FileContent, this->accessToken);
    } else {
        logger->error("Failed to read file");
    }

    file.close();
}

/**
 * @brief Handles the file download process.
 *
 * This method calls the download method to receive the file content from the server,
 * decodes it from base64, and writes it to a file specified by the filename.
 */
void Client::download() {
    std::cout << "Downloading file: " << this->filename << std::endl;
    OpenSSL_AES_Keys aesKeys;
    aesKeys.key = "p6Ix*(L/6NP)28HZ}_KQ25h@dWD+xB{^";
    aesKeys.iv = "a7fe8fed9f4v8e5d";
    this->RefreshIfNeededOrLogin();

    std::string decrypted = this->download(this->filename, aesKeys, this->accessToken);
    logger->debug("Decrypted file contents: {}", decrypted);
    // base64 decode
    std::string decoded = OpenSSL::base64_decode(decrypted);
    logger->debug("Decoded file contents: {}", decoded);
    std::ofstream file(this->filename, std::ios::binary);
    file << decoded;
    file.close();
}

/**
 * @brief Handles the user login process.
 *
 * This method prompts the user for a username and password, sends them to the server,
 * and processes the response to retrieve and store the access and refresh tokens.
 * It logs the user in if the credentials are correct.
 *
 * @throws std::runtime_error If the login fails.
 */
void Client::login() {
    spdlog::set_level(spdlog::level::debug);
    // get username and password
    std::string username;
    std::string password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    Command command(LOGIN, {OpenSSL::base64_encode(username), OpenSSL::base64_encode(password)});
    this->send(command.toString());
    std::string response = this->receiveString();
    if (response == "ERROR") {
        std::cout << "Login failed!" << std::endl;
        throw std::runtime_error("Login failed");
    }
    std::cout << "Login successful!" << std::endl;
    logger->debug("Response: {}", response);
    // parse response
    std::string access = response.substr(0, response.find(SEPARATOR));
    std::string refresh = response.substr(response.find(SEPARATOR) + 1);
    logger->debug("Access token: {}", access);
    logger->debug("Refresh token: {}", refresh);
    this->accessToken = access;
    this->refreshToken = refresh;
}

/**
 * @brief Refreshes the access token using the refresh token.
 *
 * This method sends the refresh token to the server to obtain a new access token.
 * If the refresh token is invalid, it calls the login method to authenticate again.
 */
void Client::RefreshToken() {
    Command command(REFRESH_TOKEN, {this->refreshToken});
    this->send(command.toString());
    std::string response = this->receiveString();
    if (response == "ERROR") {
        std::cout << "Refresh failed!" << std::endl;
        login();
        return;
    }
    std::cout << "Refresh successful!" << std::endl;
    logger->debug("Response: {}", response);
    // parse response
    std::string access = response.substr(0, response.find(SEPARATOR));
    std::string refresh = response.substr(response.find(SEPARATOR) + 1);
    logger->debug("Access token: {}", access);
    logger->debug("Refresh token: {}", refresh);
    this->accessToken = access;
    this->refreshToken = refresh;
}

/**
 * @brief Checks if the access token needs refreshing and refreshes it if necessary.
 *
 * This method checks if the access token is close to expiring and, if so, calls the
 * RefreshToken method to refresh it. If there is no refresh token, it calls the login
 * method to authenticate the user.
 */
void Client::RefreshIfNeededOrLogin() {
    if (!this->refreshToken.empty()) {
        auto decoded = jwt::decode(this->accessToken);
        auto exp = decoded.get_expires_at();
        auto now = std::chrono::system_clock::now();
        if (exp < now - std::chrono::seconds(60)) {
            this->RefreshToken();
        }
    } else {
        login();
    }
}


void Client::listFiles(){
    RefreshIfNeededOrLogin();
    Command command(LIST, {this->accessToken});
    this->send(command.toString());
    std::string response = this->receiveString();
    if (response == "ERROR") {
        logger->error("List failed");
        throw std::runtime_error("List failed");
    }
    logger->debug("Response: {}", response);
    auto files = Command::split(response, SEPARATOR);
    for(auto &file : files){
        std::cout << OpenSSL::base64_decode(file) << std::endl;
    }
}