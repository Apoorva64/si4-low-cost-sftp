//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SERVER_H
#define SECTRANS_SERVER_H

#include "CLI11.hpp"
#include "SocketCommunication/SocketCommunication.h"
#include "jwt-cpp/jwt.h"
#include "nlohmann/json.hpp"

struct default_clock {
    [[nodiscard]] jwt::date now() const {
        return jwt::date(std::chrono::system_clock::now());
    }
};
/**
 * @class Server
 * @brief This class represents a server in a client-server communication model.
 *
 * The Server class inherits from the SocketCommunication class.
 * It provides functionalities for handling incoming messages, SSL handshake, file operations, token management, and Keycloak resource management.
 */
class Server : public CLI::App ,public SocketCommunication {





    /**
     * @brief Handle an incoming message.
     *
     * @param msg The incoming message to handle.
     */
    void handleMessage(const std::string &msg)  override;

public:
    /**
     * @brief Construct a new Server object with a specified input port.
     *
     * @param inPort1 The input port number.
     */
    explicit Server(int inPort1);

    /**
     * @brief Construct a new Server object with specified input and output ports.
     *
     * @param inPort1 The input port number.
     * @param outPort The output port number.
     */
    Server(int inPort1, int outPort);

    /**
     * @brief Perform SSL handshake.
     *
     * @param args The arguments for the handshake.
     */
    void sslHandshake(std::vector<std::string> args);

    /**
     * @brief Upload a file.
     *
     * @param args The arguments for the file upload.
     */
    void uploadFile(std::vector<std::string> args);

    /**
     * @brief Download a file.
     *
     * @param args The arguments for the file download.
     */
    void downloadFile(std::vector<std::string> args);

    /**
     * @brief List all files.
     */
    void listFiles();

    /**
     * @brief Delete a file.
     *
     * @param args The arguments for the file deletion.
     */
    void deleteFile(std::vector<std::string> args);

    /**
     * @brief Refresh server tokens.
     */
    void refreshServerTokens();

    std::string resourceServerAccessToken;
    /**
     * @brief Create a Keycloak resource.
     *
     * @param filename The name of the file.
     * @param owner The owner of the file.
     * @return The created Keycloak resource.
     */
    nlohmann::basic_json<> createKeycloakResource(std::string filename, const std::string& owner);
    jwt::verifier<default_clock, jwt::traits::kazuho_picojson> verifier = jwt::verify<default_clock, jwt::traits::kazuho_picojson>(default_clock{}).leeway(0);

    /**
     * @brief Verify or refresh server tokens.
     */
    void verifyOrRefreshServerTokens();

    /**
     * @brief Log in.
     *
     * @param username The username.
     * @param password The password.
     * @return The login result.
     */
    nlohmann::json login(std::string username, std::string password);

    /**
     * @brief Log in.
     *
     * @param args The arguments for the login.
     */
    void login(std::vector<std::string> args);

    /**
     * @brief Refresh a token.
     *
     * @param args The arguments for the token refresh.
     */
    void refreshToken(std::vector<std::string> args);

    /**
     * @brief Refresh a token.
     *
     * @param refresh_token The refresh token.
     * @return The refreshed token.
     */
    nlohmann::json refreshToken(std::string refresh_token);

    /**
     * @brief Check permission in Keycloak.
     *
     * @param filename The name of the file.
     * @param owner The owner of the file.
     * @param permission The permission to check.
     */
    void checkPermissionKeycloak(std::string filename, const std::string &owner, std::string permission);

    /**
     * @brief Delete a Keycloak resource.
     *
     * @param filename The name of the file.
     */
    void deleteKeycloakResource(const std::string &filename);

    /**
     * @brief Add default permissions in Keycloak.
     *
     * @param resourceId The resource ID.
     * @param ownerAccessToken The owner's access token.
     */
    void addDefaultPermissionsKeycloak(std::string resourceId, const std::string &ownerAccessToken);

    /**
     * @brief Reset Keycloak.
     */
    void ResetKeycloak();

    bool doSendPort;
};


#endif //SECTRANS_SERVER_H