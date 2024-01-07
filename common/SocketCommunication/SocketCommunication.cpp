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

const char  END_OF_MESSAGE = '`';

/**
 * @brief Constructs a SocketCommunication object.
 *
 * Initializes the object with input and output port numbers, allocates buffers for reading and writing,
 * sets up a logger, and initializes the SSL negotiation flag to false.
 *
 * @param inPort The port number for incoming messages.
 * @param outPort The port number for outgoing messages.
 */
SocketCommunication::SocketCommunication(int inPort, int outPort) : inPort(inPort), outPort(outPort),
                                                                    logger(spdlog::stdout_color_mt(
                                                                            fmt::format("SocketCommunication[{},{}]",
                                                                                        inPort, outPort))) {}


/**
 * @brief Starts the server on the input port.
 *
 * Calls the external function `startserver` to begin listening for incoming connections on the input port.
 */
void SocketCommunication::start() {
    logger->info("| SocketCommunication.start | Starting server on port {}", this->inPort);
    startserver(this->inPort);
}

/**
 * @brief Tests the connection by sending a ping message.
 *
 * Sends a "Ping" message to the server and waits for a "Pong" response to confirm the connection is working.
 * Throws a runtime error if the connection test fails.
 *
 * @throws std::runtime_error If the connection test fails.
 */
void SocketCommunication::test() const {
    logger->info("| SocketCommunication.test | Testing Connection to server sending Ping on port {}", this->outPort);
    this->send("Ping");
    logger->info("| SocketCommunication.test | Waiting for response... on port {}", this->outPort);
    if (this->receiveString() != "Pong") {
        throw std::runtime_error("Connection failed");
    }
    std::cout << "Connection established!" << std::endl;
}


/**
 * @brief Wrapper for sending a message.
 *
 * Sends a message using the `sndmsg` external function and performs a handshake to ensure the message was received.
 * Throws a runtime error if sending fails or the handshake is not acknowledged.
 *
 * @param msg The message to send.
 * @param port The port number to send the message to.
 * @return int The result of the `sndmsg` call.
 * @throws std::runtime_error If sending fails or the handshake is not acknowledged.
 */
int SocketCommunication::sndmsgWrapper(char msg[1024], int port) const {
    int error = sndmsg(msg, port);
    if (error == -1) {
        logger->error("| SocketCommunication.sndmsgWrapper | Libclient error");
        throw std::runtime_error("Libclient error");
    }

    // get OK from other side
    getmsg(this->readBuffer);
    logger->debug("| SocketCommunication.sndmsgWrapper | Check Received: {}", this->readBuffer);
    if (std::string(this->readBuffer).substr(0, 2) !=
        "OK") { // TODO: THIS IS FCKED WHY DOES IT HAVE A FCKING 25 at the end???????
        logger->error("| SocketCommunication.sndmsgWrapper | Check failed");
        throw std::runtime_error("Check failed");
    }

    // send OK to other side
    std::string ok = "OK";
    strcpy(this->writeBuffer, ok.c_str());
    sndmsg(this->writeBuffer, port);
    return error;
}

/**
 * @brief Wrapper for receiving a message.
 *
 * Receives a message using the `getmsg` external function and performs a handshake to acknowledge receipt.
 * Throws a runtime error if receiving fails or the handshake is not acknowledged.
 *
 * @param msg The buffer to store the received message.
 * @param port The port number to receive the message from.
 * @return int The result of the `getmsg` call.
 * @throws std::runtime_error If receiving fails or the handshake is not acknowledged.
 */
int SocketCommunication::getmsgWrapper(char msg[1024], int port) const {
    int error = getmsg(this->tempBuffer);
    if (error == -1) {
        logger->error("| SocketCommunication.getmsgWrapper | LibServer error");
        throw std::runtime_error("LibServer error");
    }
    // send OK to other side
    std::string ok = "OK";
    strcpy(this->writeBuffer, ok.c_str());
    sndmsg(this->writeBuffer, port);

    // get OK from other side
    getmsg(this->readBuffer);
    logger->info("| SocketCommunication.getmsgWrapper | Check Received: {}", this->readBuffer);
    if (std::string(this->readBuffer).substr(0, 2) !=
        "OK") { // TODO: THIS IS FCKED WHY DOES IT HAVE A FCKING 25 at the end???????
        throw std::runtime_error("Check failed");
    }
    strcpy(msg, this->tempBuffer);
    return error;
}

/**
 * @brief Sends a message to the server.
 *
 * Encrypts and encodes the message if SSL negotiation is enabled, appends an end-of-message character,
 * and sends the message in chunks of 1024 bytes using `sndmsgWrapper`.
 *
 * @param msg The message to send.
 */
void SocketCommunication::send(const std::string &msg) const {
    logger->info("| SocketCommunication.send | Sending: {} to {}", msg, this->outPort);
    std::string msgToSend;
    if (this->isSslNegotiate) {
        msgToSend = OpenSSL::base64_encode(OpenSSL::aes_encrypt(msg, this->key->key, this->key->iv)) + END_OF_MESSAGE;
    } else {
        msgToSend = msg + END_OF_MESSAGE;
    }
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

/**
 * @brief Receives a string message from the server.
 *
 * Calls `getmsgWrapper` to receive chunks of the message until the end-of-message character is found.
 * Decrypts and decodes the message if SSL negotiation is enabled.
 *
 * @return std::string The received message.
 */
std::string SocketCommunication::receiveString() const {
    this->getmsgWrapper(this->readBuffer, this->outPort);
    std::string msg = this->readBuffer;
    logger->debug("| SocketCommunication.receiveString | Received Chunk: {}", msg);
    while (msg.find(END_OF_MESSAGE) == std::string::npos) {
        this->getmsgWrapper(this->readBuffer, this->outPort);
        msg += this->readBuffer;
    }
    std::string fullMsg = msg.substr(0, msg.find(END_OF_MESSAGE));
    if (this->isSslNegotiate) {
        fullMsg = OpenSSL::aes_decrypt(OpenSSL::base64_decode(fullMsg), this->key->key, this->key->iv);
    }
    logger->info("| SocketCommunication.receiveString | Dialog received: {}", fullMsg);
    return fullMsg;
}

/**
 * @brief Handles incoming messages.
 *
 * Responds to a "Ping" message with a "Pong" message.
 *
 * @param msg The incoming message to handle.
 */
void SocketCommunication::handleMessage(const std::string &msg) {
    if (msg == "Ping") {
        this->send("Pong");
    }
}

/**
 * @brief Runs the main loop to handle incoming messages.
 *
 * Continuously receives and handles messages in an infinite loop.
 */
[[noreturn]] void SocketCommunication::run() {
    while (true) {
        std::string msg = this->receiveString();
        this->handleMessage(msg);
    }
}

void SocketCommunication::Close() {
    stopserver();

}
