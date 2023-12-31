#define CONFIG_CATCH_MAIN
#include <catch2/catch_all.hpp>
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

TEST_CASE("Generate String", "[gen_random_string]"){
    int size = 10;
    std::string res = OpenSSL_Utils::generateRandomString(size);

    REQUIRE(res.size() == size);
}