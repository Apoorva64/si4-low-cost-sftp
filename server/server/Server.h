//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SERVER_H
#define SECTRANS_SERVER_H


#include "SocketCommunication/SocketCommunication.h"

class Server : public SocketCommunication {


    void handleMessage(const std::string &msg) const override;

public:
    explicit Server(int inPort1);

    Server(int inPort1, int outPort);

    void uploadFile(std::vector<std::string> args) const;

    void downloadFile(std::vector<std::string> args) const;

    void listFiles() const;

    void deleteFile(std::vector<std::string> args) const;

    void login(std::vector<std::string> args) const;
};


#endif //SECTRANS_SERVER_H
