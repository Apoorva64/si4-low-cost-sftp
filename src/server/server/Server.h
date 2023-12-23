//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SERVER_H
#define SECTRANS_SERVER_H


#include "../../common/SocketCommunication/SocketCommunication.h"

class Server : public SocketCommunication {


    void handleMessage(const std::string &msg) const override;

public:
    explicit Server(int inPort1);

    Server(int inPort1, int outPort);
};


#endif //SECTRANS_SERVER_H
