#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "Response/Response.h"

// Test the constructor with response string
TEST_CASE("Response Constructor with String", "[constructor]") {
    auto separator = std::string(1, SEPARATOR);
    std::string input = "0" + separator + "data1" + separator + "data2";
    Response response(input);
    std::vector<std::string> data = response.getArgs();
    REQUIRE(data.size() == 2);
    CHECK(data[0] == "data1");
    CHECK(data[1] == "data2");
}

// Test the constructor with invalid response string
TEST_CASE("Response Constructor with Invalid String", "[constructor]") {
    std::string input = "invalid_response";
    Response response(input);
    std::vector<std::string> data = response.getArgs();
    REQUIRE(data.empty());  // No valid arguments
}

// Test the getResponseEnum function
TEST_CASE("Response getResponseEnum Function", "[getResponseEnum]") {
    CHECK(Response::getResponseEnum("0") == ResponseEnum::DATA);
    CHECK(Response::getResponseEnum("1") == ResponseEnum::ERROR);
    CHECK(Response::getResponseEnum("invalid_response") == ResponseEnum::ERROR);
}

// Test the toString function
TEST_CASE("Response ToString Function", "[toString]") {
    auto separator = std::string(1, SEPARATOR);
    Response response(ResponseEnum::DATA, {"data1", "data2"});
    std::string expected = std::to_string(ResponseEnum::DATA) + separator + "data1" + separator + "data2";
    std::string result = response.toString();
    CHECK(result == expected);
}
