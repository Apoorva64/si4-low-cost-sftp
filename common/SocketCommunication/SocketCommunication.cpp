//
// Created by appad on 23/12/2023.
//

#include <iostream>
#include <cstring>
#include "SocketCommunication.h"

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

}

void SocketCommunication::start() const {
    std::cout << "Starting server..." << std::endl;
    startserver(this->inPort);
}

void SocketCommunication::test() const {
    std::cout << "Testing Connection to server..." << std::endl;
    this->send("Ping");
    std::string str = this->receiveString();
    if (str != "Pong") {
        throw std::runtime_error("Connection failed");
    }
    std::cout << "Connection established!" << std::endl;
}

int SocketCommunication::sndmsgWrapper(char msg[1024], int port) const {
    int error = sndmsg(msg, port);
    if (error == -1) {
        throw std::runtime_error("Error sending message");
    }
    // get OK from server
    memset(this->readBuffer, 0, 1024);
    getmsg(this->readBuffer);
    std::cout << "Check Received: " << this->readBuffer << std::endl;
    if (std::string(this->readBuffer).substr(0, 2) != "OK") { // TODO: THIS IS FCKED WHY DOES IT HAVE A FCKING 25 at the end???????
        throw std::runtime_error("Error sending message");
    }
    return error;
}

int SocketCommunication::getmsgWrapper(char msg[1024], int port) const {
    int error = getmsg(msg);
    if (error == -1) {
        throw std::runtime_error("Error receiving message");
    }
    // Wait for other side to be ready sleeping for 0.1 second
    usleep(100000); //TODO: THIS IS FCKED UP AND SHOULD BE REPLACED WITH A PROPER SOLUTION HEEEEEEEEEEEEEEEEEEEELP
    // send OK to server
    std::string ok = "OK";
    memset(this->writeBuffer, 0, 1024);
    strcpy(this->writeBuffer, ok.c_str());
    sndmsg(this->writeBuffer, port);
    return error;
}

void SocketCommunication::send(const std::string &msg) const {
    std::cout << "Sending: " << msg << std::endl;
    std::string msgToSend = msg + END_OF_MESSAGE;
    unsigned long msgLength = msgToSend.length();
    unsigned long chunks = msgLength / 1024;
    unsigned long remainder = msgLength % 1024;
    for (unsigned long i = 0; i < chunks; i++) {
        std::string chunk = msgToSend.substr(i * 1024, 1024);
        strcpy(this->writeBuffer, chunk.c_str());
//        sndmsg(this->writeBuffer, this->outPort);
        this->sndmsgWrapper(this->writeBuffer, this->outPort);
    }
    std::string chunk = msgToSend.substr(chunks * 1024, remainder);
    strcpy(this->writeBuffer, chunk.c_str());
//    sndmsg(this->writeBuffer, this->outPort);
    this->sndmsgWrapper(this->writeBuffer, this->outPort);

}


std::string SocketCommunication::receiveString() const {
//    getmsg(this->readBuffer);
    this->getmsgWrapper(this->readBuffer, this->outPort);
    std::string msg = this->readBuffer;
    std::cout << "Received Chunk: " << msg << std::endl;
    while (msg.find(END_OF_MESSAGE) == std::string::npos) {
//        getmsg(this->readBuffer);
        this->getmsgWrapper(this->readBuffer, this->outPort);
        msg += this->readBuffer;
    }
    std::string fullMsg = msg.substr(0, msg.find(END_OF_MESSAGE));
    std::cout << "Received: " << fullMsg << std::endl;
    return fullMsg;
}

void SocketCommunication::handleMessage(const std::string &msg) const {
    if (msg == "Ping") {
        this->send("Pong");
    }
}

[[noreturn]] void SocketCommunication::run() const {
    while (true) {
        std::string msg = this->receiveString();
        this->handleMessage(msg);
    }
}