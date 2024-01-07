//
// Created by appad on 07/01/2024.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include "Command/Command.h"

// Test the split function
TEST_CASE("Command Split Function", "[split]") {
    auto separator = std::string(1, SEPARATOR);
    std::string input = "one" + separator + "two" + separator + "three";
    std::vector<std::string> result = Command::split(input, SEPARATOR);
    REQUIRE(result.size() == 3);
    CHECK(result[0] == "one");
    CHECK(result[1] == "two");
    CHECK(result[2] == "three");
}

// Test the toString function
TEST_CASE("Command ToString Function", "[toString]") {
    auto separator = std::string(1, SEPARATOR);
    Command cmd(CommandEnum::UPLOAD, {"file.txt", "path/"});
    std::string expected = std::to_string(CommandEnum::UPLOAD) + separator + "file.txt" + separator + "path/";
    std::string result = cmd.toString();
    CHECK(result == expected);
}

// Test the constructor with command string
TEST_CASE("Command Constructor with String", "[constructor]") {
    auto separator = std::string(1, SEPARATOR);
    std::string input = "1" + separator + "file.txt" + separator + "path/";
    Command cmd(input);
    std::vector<std::string> commands = cmd.getArgs();
    REQUIRE(commands.size() == 2);
    CHECK(commands[0] == "file.txt");
    CHECK(commands[1] == "path/");
}

// Test the constructor with command enum and vector of strings
TEST_CASE("Command Constructor with Enum and Vector", "[constructor]") {
    Command cmd(CommandEnum::UPLOAD, {"file.txt", "path/"});
    std::vector<std::string> commands = cmd.getArgs();
    REQUIRE(commands.size() == 2);
    CHECK(commands[0] == "file.txt");
    CHECK(commands[1] == "path/");
}

// Test the getCommandEnum function
TEST_CASE("Command getCommandEnum Function", "[getCommandEnum]") {
    CHECK(Command::getCommandEnum("0") == CommandEnum::LOGIN);
    CHECK(Command::getCommandEnum("1") == CommandEnum::UPLOAD);
    CHECK(Command::getCommandEnum("2") == CommandEnum::DOWNLOAD);
    CHECK(Command::getCommandEnum("3") == CommandEnum::LIST);
    CHECK(Command::getCommandEnum("4") == CommandEnum::DELETE);
    CHECK(Command::getCommandEnum("7") == CommandEnum::INIT_SESSION);
    CHECK(Command::getCommandEnum("8") == CommandEnum::SSL_HANDSHAKE);
    CHECK(Command::getCommandEnum("9") == CommandEnum::SERVER_ERROR);
    CHECK(Command::getCommandEnum("unknown") == CommandEnum::UNKNOWN);
}