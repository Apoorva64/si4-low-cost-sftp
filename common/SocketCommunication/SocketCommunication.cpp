//
// Created by appad on 23/12/2023.
//

#include <iostream>
#include <cstring>
#include "SocketCommunication.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

extern "C" {
#include "libclient.h"
#include "libserver.h"
}
#include<unistd.h>

#define END_OF_MESSAGE '`'

SocketCommunication::SocketCommunication(int inPort, int outPort) {
    this->inPort = inPort;
    this->outPort = outPort;
    this->readBuffer = new char[1024];
    this->writeBuffer = new char[1024];
    this->logger = spdlog::stdout_color_mt("SocketCommunication");
    this->isSslNegotiate = true;
}

void SocketCommunication::start() const {
    logger->info("Starting server on port {}", (int) this->inPort);
    startserver(this->inPort);
}

void SocketCommunication::test() const {
    logger->info("Testing Connection to server sending Ping on port {}", (int) this->outPort);
    this->send("Ping");
    logger->info("Waiting for response... on port {}", (int) this->outPort);
    std::string str = this->receiveString();
    if (str != "Pong") {
        throw std::runtime_error("Connection failed");
    }
    std::cout << "Connection established!" << std::endl;
}

int SocketCommunication::sndmsgWrapper(char msg[1024], int port) const {
    int error = sndmsg(msg, port);
    if (error == -1) {
        logger->error("Libclient error");
        throw std::runtime_error("Libclient error");
    }

    // get OK from other side
    getmsg(this->readBuffer);
    logger->debug("Check Received: {}", this->readBuffer);
    if (std::string(this->readBuffer).substr(0, 2) != "OK") { // TODO: THIS IS FCKED WHY DOES IT HAVE A FCKING 25 at the end???????
        logger->error("Check failed");
        throw std::runtime_error("Check failed");
    }

    // send OK to other side
    std::string ok = "OK";
    strcpy(this->writeBuffer, ok.c_str());
    sndmsg(this->writeBuffer, port);
    return error;
}

int SocketCommunication::getmsgWrapper(char msg[1024], int port) const {
    char *msg2 = new char[1024];
    int error = getmsg(msg2);
    if (error == -1) {
        logger->error("LibServer error");
        throw std::runtime_error("LibServer error");
    }
    // send OK to other side
    std::string ok = "OK";
    strcpy(this->writeBuffer, ok.c_str());
    sndmsg(this->writeBuffer, port);

    // get OK from other side
    getmsg(this->readBuffer);
    logger->debug("Check Received: {}", this->readBuffer);
    if (std::string(this->readBuffer).substr(0, 2) != "OK") { // TODO: THIS IS FCKED WHY DOES IT HAVE A FCKING 25 at the end???????
        throw std::runtime_error("Check failed");
    }
    strcpy(msg, msg2);
    return error;
}

void SocketCommunication::send(const std::string &msg) const {
    logger->info("Sending: {}", msg);
    std::string msgToSend = msg + END_OF_MESSAGE;
    unsigned long msgLength = msgToSend.length();
    unsigned long chunks = msgLength / 1024;
    unsigned long remainder = msgLength % 1024;
    for (unsigned long i = 0; i < chunks; i++) {
        std::string chunk = msgToSend.substr(i * 1024, 1024);
        strcpy(this->writeBuffer, chunk.c_str());
        this->sndmsgWrapper(this->writeBuffer, this->outPort);
    }
    std::string chunk = msgToSend.substr(chunks * 1024, remainder);
    strcpy(this->writeBuffer, chunk.c_str());
    this->sndmsgWrapper(this->writeBuffer, this->outPort);

}


std::string SocketCommunication::receiveString() const {
    this->getmsgWrapper(this->readBuffer, this->outPort);
    std::string msg = this->readBuffer;
    logger->debug("Received Chunk: {}", msg);
    while (msg.find(END_OF_MESSAGE) == std::string::npos) {
        this->getmsgWrapper(this->readBuffer, this->outPort);
        msg += this->readBuffer;
    }
    std::string fullMsg = msg.substr(0, msg.find(END_OF_MESSAGE));
    logger->info("Received: {}", fullMsg);
    return fullMsg;
}

void SocketCommunication::handleMessage(const std::string &msg)  {
    if (msg == "Ping") {
        this->send("Pong");
    }
}

[[noreturn]] void SocketCommunication::run()  {
    while (true) {
        std::string msg = this->receiveString();
        this->handleMessage(msg);
    }
}