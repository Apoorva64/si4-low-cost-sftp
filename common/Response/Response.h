//
// Created by XKHC2270 on 07/01/2024.
//

#ifndef SECTRANS_RESPONSE_H
#define SECTRANS_RESPONSE_H


#include "Args/Arguments.h"
#include "Command/Command.h"

class Response : Command  {
public:

    /**
     * @brief Constructs a Command object with a command enum and arguments.
     *
     * This constructor initializes a Command object with the provided command enum value
     * and a vector of arguments.
     *
     * @param commandEnum The command enum value.
     * @param args A vector of strings representing the arguments for the command.
     */
    Response::Command(const std::string &commandString): Arguments(commandString){
        commandEnum = Response::getResponseEnum(args[0]);
        args.erase(args.begin());

    }
    Response::Command(CommandEnum commandEnum, std::vector<std::string> args): commandEnum(commandEnum), Arguments(args) {}


};


#endif //SECTRANS_RESPONSE_H
