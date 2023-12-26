//
// Created by appad on 23/12/2023.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Server.h"
#include "OpenSSL.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Command/CommandEnum.h"
#include "Command/Command.h"

#define SPERATOR '|'
#define FILES_FOLDER "files"


Server::Server(int inPort1, int outPort) : SocketCommunication(inPort1, outPort) {
    this->logger = spdlog::stdout_color_mt("Server");
    // create files folder if not exists
    std::filesystem::create_directory(FILES_FOLDER);

}

Server::Server(int inPort1) : SocketCommunication(inPort1, 0) {

}

void Server::handleMessage(const std::string &msg) const {
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
        case UNKNOWN:
            logger->error("Unknown command: {}", command.toString());
            break;
    }

}

void Server::deleteFile(std::vector<std::string> args) const {
    const std::string &filename = args[0];
    logger->info("Deleting file: {}", filename);
    std::filesystem::remove(std::string(FILES_FOLDER) + "/" + filename);
    logger->info("File deleted!");
}

void Server::listFiles() const {
    logger->info("Listing files...");
    std::string files;
    for (const auto &entry: std::filesystem::directory_iterator(FILES_FOLDER)) {
        files += entry.path().filename().string() + SPERATOR;
    }
    logger->info("Sending files...");
    send(files);
    logger->info("Files sent!");
}

void Server::downloadFile(std::vector<std::string> args) const {
    const std::string &filename = args[0];
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

void Server::uploadFile(std::vector<std::string> args) const {
    std::string filenameStr = args[0];
    std::filesystem::path path(filenameStr);
    std::string filename = path.filename();
    std::string file = args[1];
    logger->info("Uploading file: {}", filename);
    std::ofstream outfile(std::string(FILES_FOLDER) + "/" + filename);
    outfile << file;
    outfile.close();
    logger->info("File uploaded!");
}

void Server::login(std::vector<std::string> args) const {
    std::string username = args[0];
    std::string password = args[1];
    logger->info("Logging in user: {}", username);
    // decode base64
    username = OpenSSL::base64_decode(username);
    password = OpenSSL::base64_decode(password);
    logger->info("Username: {}", username);

}