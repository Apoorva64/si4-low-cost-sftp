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
    std::string accessToken;
    std::string refreshToken;
    void start() const;
    void upload();


    std::string download(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &accessToken);

    void download();

    void upload(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &base64FileContent,
                const std::string &accessToken);

    void login();

    void RefreshToken();

    void RefreshIfNeeded();
};


#endif //SECTRANS_CLIENT_H
