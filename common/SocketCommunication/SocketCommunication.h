//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SOCKETCOMMUNICATION_H
#define SECTRANS_SOCKETCOMMUNICATION_H

#include "spdlog/logger.h"
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

/**
 * @class SocketCommunication
 * @brief This class provides functionalities for socket communication.
 *
 * The SocketCommunication class provides functionalities for sending and receiving messages over a socket,
 * handling incoming messages, and running the main loop to handle incoming messages.
 */
class SocketCommunication {
public:
    /**
     * @brief Construct a new SocketCommunication object.
     *
     * @param inPort The input port number.
     * @param outPort The output port number.
     */
    SocketCommunication(int inPort, int outPort);

    char *readBuffer; ///< The buffer for reading messages.
    char *writeBuffer; ///< The buffer for writing messages.
    int inPort; ///< The input port number.
    int outPort; ///< The output port number.
    bool isSslNegotiate; ///< The flag for SSL negotiation.
    EVP_PKEY *keyClient, *keyServer; ///< The client and server keys.
    OpenSSL_AES_Keys_st *key; ///< The AES key.

    /**
     * @brief Start the server on the input port.
     */
    virtual void start();

    /**
     * @brief Send a message to the server.
     *
     * @param msg The message to send.
     */
    void send(const std::string& msg) const;

    /**
     * @brief Receive a string message from the server.
     *
     * @return The received message.
     */
    [[nodiscard]] std::string receiveString() const;

    /**
     * @brief Handle an incoming message.
     *
     * @param msg The incoming message to handle.
     */
    virtual void handleMessage(const std::string& msg);

    /**
     * @brief Test the connection by sending a ping message.
     */
    void test() const;

    /**
     * @brief Receive a message from the server.
     *
     * @param msg The buffer to store the received message.
     * @param port The port number to receive the message from.
     * @return The result of the `getmsg` call.
     */
    int getmsgWrapper(char *msg, int port) const;

    /**
     * @brief Send a message to the server.
     *
     * @param msg The message to send.
     * @param port The port number to send the message to.
     * @return The result of the `sndmsg` call.
     */
    int sndmsgWrapper(char *msg, int port) const;

    std::shared_ptr<spdlog::logger> logger; ///< The logger.

    /**
     * @brief Run the main loop to handle incoming messages.
     */
    [[noreturn]] void run();
};

#endif //SECTRANS_SOCKETCOMMUNICATION_H