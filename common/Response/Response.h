//
// Created by XKHC2270 on 07/01/2024.
//

#ifndef SECTRANS_RESPONSE_H
#define SECTRANS_RESPONSE_H


#include "Args/Arguments.h"
#include "Command/Command.h"
#include "ResponseEnum.h"

class Response : public Arguments  {
public:

    static ResponseEnum getResponseEnum(std::string basicString);

/**
     * @brief Constructs a Command object with a command enum and arguments.
     *
     * This constructor initializes a Command object with the provided command enum value
     * and a vector of arguments.
     *
     * @param commandEnum The command enum value.
     * @param args A vector of strings representing the arguments for the command.
     */
    [[maybe_unused]] explicit Response(const std::string &commandString);

    Response(ResponseEnum responseEnum, std::vector<std::string> args) : Arguments(std::move(args)), responseEnum(responseEnum) {}

    std::string toString() const override;

    ResponseEnum responseEnum;

};


#endif //SECTRANS_RESPONSE_H
