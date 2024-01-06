//
// Created by tombe on 29/12/2023.
//

#include "PeerServer.h"

#include <utility>
#include "Command/CommandEnum.h"
#include "Command/Command.h"

/**
 * @brief Handles incoming messages and executes corresponding actions.
 *
 * Overrides the handleMessage method from the SocketCommunication class to process
 * incoming messages as Command objects. It performs actions based on the command type.
 * If an INIT_SESSION command is received, it initializes a session. Logs an error for unknown commands.
 *
 * @param msg The raw string message received from the peer.
 */
void PeerServer::handleMessage(const std::string &msg) {
    try {
        SocketCommunication::handleMessage(msg);
        Command command(msg);

        switch (command.commandEnum) {
            case INIT_SESSION:
                initSession(command.args);
                break;
            default:
                logger->error("Unknown command: {}", command.toString());
                break;
        }
    }
    catch (std::exception &e) {
        logger->error("Error: {}", e.what());
        send("ERROR");
    }
}

/**
 * @brief Constructs a PeerServer with a specified input port.
 *
 * Initializes the PeerServer with a reference to a SessionStorage object and an input port.
 * Starts the thread pool with a capacity based on the session storage's capacity.
 *
 * @param st Pointer to the SessionStorage object.
 * @param inPort1 The input port number for the server.
 */
PeerServer::PeerServer(SessionStorage *st, int inPort1) : SocketCommunication(inPort1, 0), storage(st), threadPool(){
    threadPool.start(st->getSessionCapacity());
}

/**
 * @brief Constructs a PeerServer with specified input and output ports.
 *
 * Initializes the PeerServer with a reference to a SessionStorage object, an input port, and an output port.
 * Starts the thread pool with a capacity based on the session storage's capacity.
 *
 * @param st Pointer to the SessionStorage object.
 * @param inPort1 The input port number for the server.
 * @param outPort The output port number for the server.
 */
PeerServer::PeerServer(SessionStorage *st, int inPort1, int outPort) : SocketCommunication(inPort1, outPort), storage(st),
                                                                       threadPool(){
    threadPool.start(st->getSessionCapacity());
}

/**
 * @brief Initializes a session for a client.
 *
 * Creates a new session using the SessionStorage object, queues a job to manage the client,
 * and sends the session information back to the client.
 *
 * @param args A vector of strings containing the arguments for the INIT_SESSION command.
 */
void PeerServer::initSession(std::vector<std::string> args) {
    if(args.empty()){
        logger->error("Missing port");
        send("ERROR");
        return;
    }

    std::string port = args.at(0);

    int idSession = this->storage->generateSession(std::stoi(port));
    this->threadPool.queueJob(PeerServer::manageClient, idSession);
    logger->info("Session {} created", idSession);
}

/**
 * @brief Manages a client session.
 *
 * Static method that retrieves a session from the session storage and starts a sub-server
 * to handle communication with the client for that session.
 *
 * @param idSession The session ID for the client to manage.
 */
void PeerServer::manageClient(int idSession) {
    SESSION_CLIENT sessionClient;
    std::mutex lock;
    lock.lock();
    sessionClient = sessionStorage->getSession(idSession);
    lock.unlock();

    // start server with subprocess
    std::string command = "./SecTransServer start -p " + std::to_string(sessionClient.serverPort) + " -o " + std::to_string(sessionClient.clientPort) + " -s 1 -c 1";
    std::system(command.c_str());
}
