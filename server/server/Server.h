//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SERVER_H
#define SECTRANS_SERVER_H


#include "SocketCommunication/SocketCommunication.h"
#include "jwt-cpp/jwt.h"
#include "nlohmann/json.hpp"

class Server : public SocketCommunication {


    void handleMessage(const std::string &msg) const override;

public:
    explicit Server(int inPort1);

    Server(int inPort1, int outPort);

    void uploadFile(std::vector<std::string> args);

    void downloadFile(std::vector<std::string> args) const;

    void listFiles() const;

    void deleteFile(std::vector<std::string> args) const;

    void login(std::vector<std::string> args) const;

    void refreshServerTokens();

    std::string ressourceServerAccessToken = "";



    void createKeycloakResource(std::string filename, const std::string& owner);


    nlohmann::json_abi_v3_11_2::basic_json<> login(std::string username, std::string password) const;

    jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson> verifier;

    void verifyOrRefreshServerTokens();
};


#endif //SECTRANS_SERVER_H
