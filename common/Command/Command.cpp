//
// Created by appad on 26/12/2023.
//

#include "Command.h"

/**
 * @brief Splits a string into a vector of strings based on a delimiter.
 *
 * This static method takes a string and a delimiter character, then splits the string
 * into a vector of substrings wherever the delimiter occurs, returning the vector.
 *
 * @param str The string to be split.
 * @param delimiter The character used to split the string.
 * @return std::vector<std::string> A vector containing the split substrings.
 */
std::vector<std::string> Command::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Converts the Command object to a string representation.
 *
 * This method constructs a string that represents the Command object by concatenating
 * the command enum value with its arguments, separated by the defined SPERATOR character.
 *
 * @return std::string A string representation of the Command object.
 */
std::string Command::toString() const {
    std::string result = std::to_string(commandEnum);
    for (const auto& arg : args) {
        result += SEPARATOR + arg;
    }
    return result;
}

/**
 * @brief Constructs a Command object with a command enum and arguments.
 *
 * This constructor initializes a Command object with the provided command enum value
 * and a vector of arguments.
 *
 * @param commandEnum The command enum value.
 * @param args A vector of strings representing the arguments for the command.
 */
Command::Command(const std::string &commandString) {
    std::string command = commandString.substr(0, commandString.find(SEPARATOR));
    commandEnum = getCommandEnum(command);
    args = split(commandString.substr(commandString.find(SEPARATOR) + 1), SEPARATOR);

}

Command::Command(CommandEnum commandEnum, std::vector<std::string> args) : commandEnum(commandEnum), args(std::move(args)) {}

/**
 * @brief Retrieves the CommandEnum value corresponding to a string.
 *
 * This static method maps a string to its corresponding CommandEnum value using a
 * predefined map. If the string does not match any command, it returns UNKNOWN.
 *
 * @param command The string representation of the command.
 * @return CommandEnum The corresponding CommandEnum value.
 */
CommandEnum Command::getCommandEnum(const std::string &command) {
    static const std::unordered_map<std::string, CommandEnum> commandMap = {
            {"0", LOGIN},
            {"1", UPLOAD},
            {"2", DOWNLOAD},
            {"3", LIST},
            {"4", DELETE},
            {"7", INIT_SESSION},
            {"8", SSL_HANDSHAKE},
    };

    auto it = commandMap.find(command);
    return (it != commandMap.end()) ? it->second : UNKNOWN;
}
