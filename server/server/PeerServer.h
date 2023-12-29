//
// Created by tombe on 29/12/2023.
//

#ifndef SECTRANS_PEERSERVER_H
#define SECTRANS_PEERSERVER_H

#include "SocketCommunication/SocketCommunication.h"
#include "SessionStorage.h"
#include "ThreadPool.h"

class PeerServer : public SocketCommunication {
    void handleMessage(const std::string &msg)  override;

public:
    explicit PeerServer(SessionStorage *st, int inPort1);

    PeerServer(SessionStorage *st, int inPort1, int outPort);

    void initSession(std::vector<std::string> args);
    static void manageClient(int idSession);

private:
    SessionStorage *storage;
    ThreadPool threadPool;
};


#endif //SECTRANS_PEERSERVER_H
