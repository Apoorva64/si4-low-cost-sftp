//
// Created by appad on 23/12/2023.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Server.h"
#include "OpenSSL.h"

#define SPERATOR '|'
#define FILES_FOLDER "files"


Server::Server(int inPort1, int outPort) : SocketCommunication(inPort1, outPort) {
    // create files folder if not exists
    std::filesystem::create_directory(FILES_FOLDER);

}

Server::Server(int inPort1) : SocketCommunication(inPort1, 0) {

}

void Server::handleMessage(const std::string& msg) const {
    SocketCommunication::handleMessage(msg);
    std::string command = msg.substr(0, msg.find(SPERATOR));
    std::string args = msg.substr(msg.find(SPERATOR) + 1);
    if (command == "upload") {
        std::string filenameStr = args.substr(0, args.find(SPERATOR));
        std::filesystem::path path(filenameStr);
        std::string filename = path.filename();
        std::string file = args.substr(args.find(SPERATOR) + 1);
        std::cout << "Uploading file: " << filename << std::endl;
        std::ofstream outfile(std::string(FILES_FOLDER) + "/" + filename);
        outfile << file;
        outfile.close();
        std::cout << "File uploaded!" << std::endl;
    } else if (command == "download") {
        const std::string& filename = args;
        std::cout << "Downloading file: " << filename << std::endl;
        std::ifstream file(std::string(FILES_FOLDER) + "/" + filename, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "File not found" << std::endl;
            return;
        }
        file.seekg(0, std::ios::end);
        unsigned long fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        std::cout << "File size: " << fileSize << std::endl;
        std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::cout << "File contents: " << fileContents << std::endl;
        this->send(fileContents);
        std::cout << "File contents sent!" << std::endl;
    } else if (command == "list") {
        std::cout << "Listing files..." << std::endl;
        std::string files;
        for (const auto & entry : std::filesystem::directory_iterator(FILES_FOLDER)) {
            files += entry.path().filename().string() + SPERATOR;
        }
        std::cout << "Sending files..." << std::endl;
        this->send(files);
        std::cout << "Files sent!" << std::endl;
    }

    else {
        std::cout << "Unknown command: " << command << std::endl;
    }

}