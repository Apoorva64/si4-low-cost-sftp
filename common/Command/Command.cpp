//
// Created by appad on 26/12/2023.
//

#include "Command.h"

std::vector<std::string> Command::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string Command::toString() const {
    std::string result = std::to_string(commandEnum);
    for (const auto& arg : args) {
        result += SPERATOR + arg;
    }
    return result;
}

Command::Command(const std::string &commandString) {
    std::string command = commandString.substr(0, commandString.find(SPERATOR));
    commandEnum = getCommandEnum(command);
    args = split(commandString.substr(commandString.find(SPERATOR) + 1), SPERATOR);

}

Command::Command(CommandEnum commandEnum, std::vector<std::string> args) : commandEnum(commandEnum), args(std::move(args)) {}

CommandEnum Command::getCommandEnum(const std::string &command) {
    static const std::unordered_map<std::string, CommandEnum> commandMap = {
            {"0", LOGIN},
            {"1", UPLOAD},
            {"2", DOWNLOAD},
            {"3", LIST},
            {"4", DELETE},
    };

    auto it = commandMap.find(command);
    return (it != commandMap.end()) ? it->second : UNKNOWN;
}
