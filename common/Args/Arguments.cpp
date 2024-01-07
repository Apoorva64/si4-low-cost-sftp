//
// Created by XKHC2270 on 07/01/2024.
//

#include "Arguments.h"
#include "Command/Command.h"


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
std::vector<std::string> Arguments::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Converts the Arguments object to a string representation.
 *
 * This method constructs a string that represents the Command object by concatenating
 * the command enum value with its arguments, separated by the defined SEPARATOR character.
 *
 * @return std::string A string representation of the Command object.
 */
std::string Arguments::toString() const {
    std::string result = "";
    for (const auto& arg : args) {
        result += SEPARATOR + arg;
    }
    return result;
}

Arguments::Arguments (const std::string &argumentString): args(split(argumentString,SEPARATOR)) {};

Arguments::Arguments( std::vector<std::string> args) :  args(std::move(args)) {}