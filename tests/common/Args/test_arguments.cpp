#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include "Command/Command.h"
#include "Args/Arguments.h"

// Test the toString function
TEST_CASE("ToString Function", "[toString]") {
    auto separator = std::string(1, SEPARATOR);
    Arguments args({"one", "two", "three"});
    std::string expected = separator + "one" + separator + "two" + separator + "three";
    std::string result = args.toString();
    CHECK(result == expected);
}

// Test the constructor with argument string
TEST_CASE("Constructor with String", "[constructor]") {
    auto separator = std::string(1, SEPARATOR);
    std::string input = "one" + separator + "two" + separator + "three";
    Arguments args(input);
    std::vector<std::string> arguments = args.getArgs();
    REQUIRE(arguments.size() == 3);
    CHECK(arguments[0] == "one");
    CHECK(arguments[1] == "two");
    CHECK(arguments[2] == "three");
}

// Test the constructor with vector of strings
TEST_CASE("Constructor with Vector", "[constructor]") {
    auto separator = std::string(1, SEPARATOR);
    Arguments args({"one", "two", "three"});
    std::string expected = separator + "one" + separator + "two" + separator + "three";
    std::string result = args.toString();
    CHECK(result == expected);
}
