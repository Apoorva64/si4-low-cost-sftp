//
// Created by appad on 23/12/2023.
//

#include "Client.h"
#include "AES.h"
#include "OpenSSL.h"
#include <crypt.h>
#include "CLI11.hpp"
#include "curl/curl.h"


Client::Client(int inPort, int outPort, int argc, char **argv) : SocketCommunication(inPort, outPort) {

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
    std::cout << "Uploading file: " << filename << std::endl;
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
    std::cout << "File contents: " << paddedFileContent << std::endl;
    std::cout << "File contents size: " << paddedFileContentSize << std::endl;

    std::string buf(reinterpret_cast<char *>(paddedFileContent));

    std::string env_buf = OpenSSL::aes_encrypt(buf, param.key, param.iv);
    // encrypt file contents
    unsigned char *encryptedFileContent = OpenSSL_Utils::convert_string_to_uchar(env_buf);
    // encode file contents to base64
    std::string encodedFileContent = OpenSSL::base64_encode(
            std::string((char *) encryptedFileContent, paddedFileContentSize));
    std::cout << "File contents: " << encodedFileContent << std::endl;
    this->send("upload|" + filename_ + "|" + encodedFileContent);
}

std::string Client::download(const std::string &filename_, const OpenSSL_AES_Keys &param) {
    std::cout << "Downloading file: " << filename << std::endl;
    this->send("download|" + filename_);
    std::string fileContents = this->receiveString();
    std::cout << "File contents: " << fileContents << std::endl;

    // decode base64
    std::string decodedFileContent = OpenSSL::base64_decode(fileContents);
    std::cout << "Decoded file contents: " << decodedFileContent << std::endl;

    // decrypt file contents
    std::string dec_buf = OpenSSL::aes_decrypt(decodedFileContent, param.key, param.iv);
    std::cout << "Decrypted file contents: " << dec_buf << std::endl;
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
        std::cout << "File contents: " << fileContent << std::endl;
        std::cout << "File contents size: " << size << std::endl;
        std::string fileContentString(reinterpret_cast<char *>(fileContent), size);
        std::string base64FileContent = OpenSSL::base64_encode(fileContentString);
        this->upload(this->filename, aesKeys, base64FileContent);
    } else {
        std::cout << "Failed to read file" << std::endl;
    }

    file.close();
}

void Client::download() {
    std::cout << "Downloading file: " << this->filename << std::endl;
    OpenSSL_AES_Keys aesKeys;
    aesKeys.key = "p6Ix*(L/6NP)28HZ}_KQ25h@dWD+xB{^";
    aesKeys.iv = "a7fe8fed9f4v8e5d";
    std::string decrypted = this->download(this->filename, aesKeys);
    std::cout << "Decrypted file contents: " << decrypted << std::endl;
    // base64 decode
    std::string decoded = OpenSSL::base64_decode(decrypted);
    std::cout << "Decoded file contents: " << decoded << std::endl;
    std::ofstream file(this->filename, std::ios::binary);
    file << decoded;
    file.close();
}

void Client::login() {
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
    curl = curl_easy_init();
    if (curl) {

        username = curl_easy_escape(curl, username.c_str(), (int) username.length());
        password = curl_easy_escape(curl, password.c_str(), (int) password.length());
        client_id = curl_easy_escape(curl, client_id.c_str(), (int) client_id.length());
        client_secret = curl_easy_escape(curl, client_secret.c_str(), (int) client_secret.length());
        grant_type = curl_easy_escape(curl, grant_type.c_str(), (int) grant_type.length());
        scope = curl_easy_escape(curl, scope.c_str(), (int) scope.length());

        std::string postFields = "username=" + username + "&password=" + password + "&client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=" + grant_type + "&scope=" + scope;
        std::cout << "Post fields: " << postFields << std::endl;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        res = curl_easy_perform(curl);
        std::cout << "Curl response: " << res << std::endl;
        if (res != CURLE_OK) {
            std::cout << "Curl error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);

    }
    std::cout << "Login successful!" << std::endl;
}


