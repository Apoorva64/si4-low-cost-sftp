//
// Created by tombe on 29/12/2023.
//

#include "PeerServer.h"

#include <utility>
#include "Command/CommandEnum.h"
#include "Command/Command.h"

#define SPERATOR '|'

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

PeerServer::PeerServer(SessionStorage *st, int inPort1) : SocketCommunication(inPort1, 0), storage(st), threadPool(){
    threadPool.start(st->getSessionCapacity());
}

PeerServer::PeerServer(SessionStorage *st, int inPort1, int outPort) : SocketCommunication(inPort1, outPort), storage(st),
                                                                       threadPool(){
    threadPool.start(st->getSessionCapacity());
}

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

void PeerServer::manageClient(int idSession) {
    SESSION_CLIENT sessionClient;
    std::mutex lock;
    lock.lock();
    sessionClient = sessionStorage->getSession(idSession);
    lock.unlock();

    // start server with subprocess
    std::string command = "./SecTransServer -p " + std::to_string(sessionClient.serverPort) + " -o " + std::to_string(sessionClient.clientPort) + " -s 1";
    std::system(command.c_str());


//    auto subServer = Server(sessionClient.serverPort, sessionClient.clientPort);
//
//    subServer.start();
//    subServer.send(std::to_string(sessionClient.serverPort));
//    subServer.run();
}
