//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_CLIENT_H
#define SECTRANS_CLIENT_H

#include "OpenSSL.h"
#include "OpenSSL_Utils.h"
#include "CLI11.hpp"
#include "../../common/SocketCommunication/SocketCommunication.h"

/**
 * @class Client
 * @brief This class represents a client in a client-server communication model.
 *
 * The Client class inherits from the CLI::App and SocketCommunication classes.
 * It provides functionalities for starting a client, negotiating with the server,
 * uploading and downloading files, logging in, refreshing tokens, and deleting files.
 */
class Client : public CLI::App, public SocketCommunication {

    std::string filename; ///< The name of the file to be uploaded or downloaded.

public:
    /**
     * @brief Construct a new Client object.
     *
     * @param inPort The input port number.
     * @param outPort The output port number.
     * @param argc The number of command-line arguments.
     * @param argv The command-line arguments.
     */
    Client(int inPort, int outPort, int argc, char **argv);

    std::string accessToken; ///< The access token for the client.
    std::string refreshToken; ///< The refresh token for the client.

    /**
     * @brief Start the client.
     */
    void start() override;

    /**
     * @brief Negotiate with the server.
     */
    void negotiate();

    /**
     * @brief Upload a file to the server.
     */
    void upload();

    /**
     * @brief Download a file from the server.
     *
     * @param filename_ The name of the file to be downloaded.
     * @param param The OpenSSL_AES_Keys object.
     * @param accessToken The access token.
     * @return The downloaded file as a string.
     */
    std::string download(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &accessToken);

    /**
     * @brief Download a file from the server.
     */
    void download();

    /**
     * @brief Upload a file to the server.
     *
     * @param filename_ The name of the file to be uploaded.
     * @param param The OpenSSL_AES_Keys object.
     * @param base64FileContent The content of the file in base64 format.
     * @param accessToken The access token.
     */
    void upload(const std::string &filename_, const OpenSSL_AES_Keys &param, const std::string &base64FileContent,
                const std::string &accessToken);

    /**
     * @brief Log in to the server.
     */
    void login();

    /**
     * @brief Refresh the access and refresh tokens.
     */
    void RefreshToken();

    /**
     * @brief Refresh the tokens if needed, or log in to the server.
     */
    void RefreshIfNeededOrLogin();

    /**
     * @brief Delete a file from the server.
     *
     * @param filename_ The name of the file to be deleted.
     */
    void deleteFile(std::string filename_);

    void listFiles();
};


#endif //SECTRANS_CLIENT_H