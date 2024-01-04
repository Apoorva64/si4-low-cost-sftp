//
// Created by tombe on 29/12/2023.
//

#ifndef SECTRANS_SESSIONSTORAGE_H
#define SECTRANS_SESSIONSTORAGE_H

#include "OpenSSL.h"

struct session_st{
    int clientPort, serverPort;
};

typedef struct session_st SESSION_CLIENT;


class SessionStorage {
public:
    SessionStorage(int minPort, int maxPort);

    int generateSession(int clientPort);

    SESSION_CLIENT getSession(int id);
    [[nodiscard]] int getSessionCapacity() const;
private:
    std::vector<SESSION_CLIENT> sessionList;
    int minPort, maxPort, currentFreePort;
};

extern SessionStorage *sessionStorage;

#endif //SECTRANS_SESSIONSTORAGE_H
