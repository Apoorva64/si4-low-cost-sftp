//
// Created by tombe on 29/12/2023.
//

#ifndef SECTRANS_PEERSERVER_H
#define SECTRANS_PEERSERVER_H

#include "SocketCommunication/SocketCommunication.h"
#include "../SessionStorage/SessionStorage.h"
#include "ThreadPool.h"

/**
 * @class PeerServer
 * @brief This class represents a peer server in a peer-to-peer communication model.
 *
 * The PeerServer class inherits from the SocketCommunication class.
 * It provides functionalities for handling incoming messages, initializing sessions, and managing clients.
 */
class PeerServer : public SocketCommunication {
    /**
     * @brief Handle an incoming message.
     *
     * @param msg The incoming message to handle.
     */
    void handleMessage(const std::string &msg)  override;

public:
    /**
     * @brief Construct a new PeerServer object.
     *
     * @param st The session storage.
     * @param inPort1 The input port number.
     */
    explicit PeerServer(SessionStorage *st, int inPort1);

    /**
     * @brief Construct a new PeerServer object.
     *
     * @param st The session storage.
     * @param inPort1 The input port number.
     * @param outPort The output port number.
     */
    PeerServer(SessionStorage *st, int inPort1, int outPort);

    /**
     * @brief Initialize a session.
     *
     * @param args The arguments for the session.
     */
    void initSession(std::vector<std::string> args);

    /**
     * @brief Manage a client.
     *
     * @param idSession The session ID of the client.
     */
    static void manageClient(int idSession);

private:
    SessionStorage *storage; ///< The session storage.
    ThreadPool threadPool; ///< The thread pool.
};


#endif //SECTRANS_PEERSERVER_H