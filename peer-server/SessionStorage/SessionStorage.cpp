//
// Created by tombe on 29/12/2023.
//

#include "SessionStorage.h"

SessionStorage::SessionStorage(int minPort, int maxPort) : minPort(minPort), maxPort(maxPort), currentFreePort(minPort), sessionList(maxPort-minPort) {
}

int SessionStorage::generateSession(int clientPort) {
    if(this->currentFreePort > this->maxPort){
        throw std::runtime_error("Out of port !");
    }

    SESSION_CLIENT new_session;

    new_session.clientPort = clientPort;
    new_session.serverPort = this->currentFreePort;

    int index = this->currentFreePort - this->minPort;

    this->sessionList[index] = new_session;

    this->currentFreePort++;

    return index;
}

SESSION_CLIENT SessionStorage::getSession(int id) {
    return this->sessionList[id];
}

int SessionStorage::getSessionCapacity() const {
    return this->maxPort-this->minPort;
}
