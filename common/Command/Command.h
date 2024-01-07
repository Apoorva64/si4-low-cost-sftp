//
// Created by appad on 26/12/2023.
//

#ifndef SECTRANS_COMMAND_H
#define SECTRANS_COMMAND_H

#include <utility>
#include <vector>
#include <sstream>
#include "CommandEnum.h"
#include "Args/Arguments.h"

const char SEPARATOR = '|'; ///< The separator for the command string.

/**
 * @class Command
 * @brief This class represents a command in the client-server communication model.
 *
 * The Command class provides functionalities for creating a command, converting a command to a string,
 * splitting a string into a vector of strings based on a delimiter, and getting the command enumeration value.
 */
class Command : Arguments {
public:
    CommandEnum commandEnum;
    /**
     * @brief Construct a new Command object.
     *
     * @param commandEnum The enumeration value of the command.
     * @param args The arguments of the command.
     */
    Command(CommandEnum commandEnum, std::vector<std::string> args);

    /**
     * @brief Construct a new Command object from a command string.
     *
     * @param commandString The command string.
     */
    explicit Command(const std::string &commandString);

    /**
     * @brief Convert the command to a string.
     *
     * @return The command as a string.
     */
    [[nodiscard]] std::string toString() const;

    /**
     * @brief Split a string into a vector of strings based on a delimiter.
     *
     * @param str The string to be split.
     * @param delimiter The delimiter.
     * @return The vector of strings.
     */
    static std::vector<std::string> split(const std::string &str, char delimiter);

    /**
     * @brief Get the command enumeration value from a command string.
     *
     * @param command The command string.
     * @return The command enumeration value.
     */
    static CommandEnum getCommandEnum(const std::string &command);

};


#endif //SECTRANS_COMMAND_H