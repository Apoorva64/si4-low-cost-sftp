//
// Created by XKHC2270 on 07/01/2024.
//

#include "Response.h"

Response::Response(const std::string &commandString) : Arguments(commandString){
    responseEnum = Response::getResponseEnum(args[0]);
    args.erase(args.begin());
}

ResponseEnum Response::getResponseEnum(std::string basicString) {
    if (basicString == "0") {
        return ResponseEnum::DATA;
    }
    return ResponseEnum::ERROR;
}

std::string Response::toString() const {
    std::string result = std::to_string(responseEnum);
    for (const auto& arg : args) {
        result += SEPARATOR + arg;
    }
    return result;
}
