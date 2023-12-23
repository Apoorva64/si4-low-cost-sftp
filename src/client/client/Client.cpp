//
// Created by appad on 23/12/2023.
//

#include "Client.h"
#include "AES.h"
#include "OpenSSL.h"
#include <crypt.h>
#include "CLI11.hpp"


Client::Client(int inPort, int outPort, int argc, char **argv) : SocketCommunication(inPort, outPort) {
//
//    App *upload = this->add_subcommand("upload", "Upload a file");
//    upload->add_option("-f,--file", this->filename, "Specify file to upload")->required();
//    upload->callback([this]() {
//        this->upload();
//    });
//
//
//    App *download = this->add_subcommand("download", "Download a file");
//    App *list = this->add_subcommand("list", "List files on server");
//    App *help = this->add_subcommand("help", "Display help");
//    download->add_option("-f,--file", this->filename, "Specify file to download")->required();
}

void Client::upload() {
    std::cout << "Uploading file: " << this->filename << std::endl;

}

void Client::upload(const std::string &filename_, const unsigned char *key_, const unsigned char *fileContent_) {
    AES aes = AES();
    std::cout << "Uploading file: " << filename << std::endl;
    unsigned int fileContentSize = strlen((char *) fileContent_);

    // pad file contents to be multiple of 16
    unsigned int paddedFileContentSize = fileContentSize;
    if (paddedFileContentSize % 16 != 0) {
        paddedFileContentSize += 16 - (paddedFileContentSize % 16);
    }
    auto *paddedFileContent = new unsigned char[paddedFileContentSize];
    memcpy(paddedFileContent, fileContent_, fileContentSize);
    for (unsigned int i = fileContentSize; i < paddedFileContentSize; i++) {
        paddedFileContent[i] = '\0';
    }
    std::cout << "File contents: " << paddedFileContent << std::endl;
    std::cout << "File contents size: " << paddedFileContentSize << std::endl;

    // encrypt file contents
    unsigned char *encryptedFileContent = aes.EncryptECB(paddedFileContent, paddedFileContentSize, key_);
    // encode file contents to base64
    std::string encodedFileContent = OpenSSL::base64_encode(
            std::string((char *) encryptedFileContent, paddedFileContentSize));
    std::cout << "File contents: " << encodedFileContent << std::endl;
    this->send("upload|" + filename_ + "|" + encodedFileContent);
}

void Client::download(const std::string &filename_, const unsigned char *key_) {
    std::cout << "Downloading file: " << filename << std::endl;
    this->send("download|" + filename_);
    std::string fileContents = this->receiveString();
    std::cout << "File contents: " << fileContents << std::endl;

    // decode base64
    std::string decodedFileContent = OpenSSL::base64_decode(fileContents);
    std::cout << "Decoded file contents: " << decodedFileContent << std::endl;

    // decrypt file contents
    AES aes = AES();
    unsigned char *decryptedFileContent = aes.DecryptECB((unsigned char *) decodedFileContent.c_str(),
                                                         decodedFileContent.length(), key_);
    std::cout << "Decrypted file contents: " << decryptedFileContent << std::endl;
}

