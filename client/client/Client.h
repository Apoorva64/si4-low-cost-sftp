//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_CLIENT_H
#define SECTRANS_CLIENT_H

#include "OpenSSL.h"
#include "OpenSSL_Utils.h"
#include "CLI11.hpp"
#include "../../common/SocketCommunication/SocketCommunication.h"

class Client : public CLI::App, public SocketCommunication {

    std::string filename;


public:

    Client(int inPort, int outPort, int argc, char **argv);
    void upload();

    void upload(const std::string &filename_, OpenSSL_AES_Keys param, const unsigned char *fileContent_);

    void download(const std::string &filename_, OpenSSL_AES_Keys param);
};


#endif //SECTRANS_CLIENT_H
