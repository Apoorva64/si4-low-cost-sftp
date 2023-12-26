//
// Created by appad on 26/12/2023.
//

#ifndef SECTRANS_COMMAND_H
#define SECTRANS_COMMAND_H
#define SPERATOR '|'

#include <utility>
#include <vector>
#include <sstream>

#include "CommandEnum.h"

class Command {
public:
    CommandEnum commandEnum;
    std::vector<std::string> args;

    Command(CommandEnum commandEnum, std::vector<std::string> args);

    explicit Command(const std::string &commandString);

    [[nodiscard]] std::string toString() const;

    static std::vector<std::string> split(const std::string &str, char delimiter);

    static CommandEnum getCommandEnum(const std::string &command);

};


#endif //SECTRANS_COMMAND_H
