//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SOCKETCOMMUNICATION_H
#define SECTRANS_SOCKETCOMMUNICATION_H

#include "spdlog/logger.h"
#include "OpenSSL.h"

class SocketCommunication {
public:
    SocketCommunication(int inPort, int outPort);
    char *readBuffer;
    char *writeBuffer;
    int inPort;
    int outPort;
    bool isSslNegotiate;
    EVP_PKEY *keyClient, *keyServer;
    OpenSSL_AES_Keys_st *key;


    virtual void start();

    void send(const std::string& msg) const;

    [[nodiscard]] std::string receiveString() const;

    virtual void handleMessage(const std::string& msg) ;


    void test() const;

    int getmsgWrapper(char *msg, int port) const;

    int sndmsgWrapper(char *msg, int port) const;

    std::shared_ptr<spdlog::logger> logger;

    [[noreturn]] void run();
};



#endif //SECTRANS_SOCKETCOMMUNICATION_H
