//
// Created by appad on 26/12/2023.
//

#ifndef SECTRANS_COMMANDENUM_H
#define SECTRANS_COMMANDENUM_H


#include <string>
#include <unordered_map>

/**
 * @enum CommandEnum
 * @brief This enumeration represents the different types of commands in the client-server communication model.
 *
 * The CommandEnum enumeration provides the following command types:
 * LOGIN, UPLOAD, DOWNLOAD, LIST, DELETE, UNKNOWN, REFRESH_TOKEN, INIT_SESSION, SSL_HANDSHAKE.
 */
enum CommandEnum {
    LOGIN = 0,
    UPLOAD = 1,
    DOWNLOAD = 2,
    LIST = 3,
    DELETE = 4,
    UNKNOWN = 5,
    REFRESH_TOKEN = 6,
    INIT_SESSION = 7,
    SSL_HANDSHAKE = 8,
};

#endif //SECTRANS_COMMANDENUM_H
