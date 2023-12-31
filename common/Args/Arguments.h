//
// Created by XKHC2270 on 07/01/2024.
//

#ifndef SECTRANS_ARGUMENTS_H
#define SECTRANS_ARGUMENTS_H


#include <string>
#include <utility>
#include <vector>
#include <sstream>
class Arguments {
public:

    static std::vector<std::string> split(const std::string &str, char delimiter);
    /**
 * @brief Construct a new Command object.
 *
 * @param commandEnum The enumeration value of the command.
 * @param args The arguments of the command.
 */
    explicit Arguments(std::vector<std::string> args);

    /**
     * @brief Construct a new Command object from a command string.
     *
     * @param commandString The command string.
     */
    explicit Arguments (const std::string &argumentString);

    std::vector<std::string> args;


    //get args
    virtual std::vector<std::string> getArgs() const;

    virtual std::string toString() const;
};


#endif //SECTRANS_ARGUMENTS_H
