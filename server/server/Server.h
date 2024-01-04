//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SERVER_H
#define SECTRANS_SERVER_H


#include "SocketCommunication/SocketCommunication.h"
#include "jwt-cpp/jwt.h"
#include "nlohmann/json.hpp"
struct default_clock {
    [[nodiscard]] jwt::date now() const {
        return jwt::date(std::chrono::system_clock::now());
    }
};
class Server : public SocketCommunication {


    void handleMessage(const std::string &msg)  override;

public:
    explicit Server(int inPort1);

    Server(int inPort1, int outPort);

    void sslHandshake(std::vector<std::string> args);

    void uploadFile(std::vector<std::string> args);

    void downloadFile(std::vector<std::string> args) ;

    void listFiles() ;

    void deleteFile(std::vector<std::string> args) ;


    void refreshServerTokens();

    std::string resourceServerAccessToken;



    nlohmann::basic_json<> createKeycloakResource(std::string filename, const std::string& owner);



    jwt::verifier<default_clock, jwt::traits::kazuho_picojson> verifier = jwt::verify<default_clock, jwt::traits::kazuho_picojson>(default_clock{}).leeway(0);

    void verifyOrRefreshServerTokens();

    nlohmann::json login(std::string username, std::string password);

    void login(std::vector<std::string> args);

    void refreshToken(std::vector<std::string> args);

    nlohmann::json refreshToken(std::string refresh_token);


    void checkPermissionKeycloak(std::string filename, const std::string &owner, std::string permission);


    void deleteKeycloakResource(const std::string &filename);

    void addDefaultPermissionsKeycloak(std::string resourceId, const std::string &ownerAccessToken);

    void ResetKeycloak();
};


#endif //SECTRANS_SERVER_H
