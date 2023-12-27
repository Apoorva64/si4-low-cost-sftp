//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SERVER_H
#define SECTRANS_SERVER_H


#include "SocketCommunication/SocketCommunication.h"
#include "jwt-cpp/jwt.h"
#include "nlohmann/json.hpp"

class Server : public SocketCommunication {


    void handleMessage(const std::string &msg)  override;

public:
    explicit Server(int inPort1);

    Server(int inPort1, int outPort);

    void uploadFile(std::vector<std::string> args);

    void downloadFile(std::vector<std::string> args) ;

    void listFiles() ;

    void deleteFile(std::vector<std::string> args) ;


    void refreshServerTokens();

    std::string resourceServerAccessToken;



    void createKeycloakResource(std::string filename, const std::string& owner);



    jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson> verifier = jwt::verify();

    void verifyOrRefreshServerTokens();

    nlohmann::json login(std::string username, std::string password);

    void login(std::vector<std::string> args);

};


#endif //SECTRANS_SERVER_H
