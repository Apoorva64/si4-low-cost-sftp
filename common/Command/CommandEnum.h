//
// Created by appad on 26/12/2023.
//

#ifndef SECTRANS_COMMANDENUM_H
#define SECTRANS_COMMANDENUM_H


#include <string>
#include <unordered_map>

enum CommandEnum {
    LOGIN = 0,
    UPLOAD = 1,
    DOWNLOAD = 2,
    LIST = 3,
    DELETE = 4,
    UNKNOWN = 5,
};

#endif //SECTRANS_COMMANDENUM_H
