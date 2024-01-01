#define CONFIG_CATCH_MAIN
#include <catch2/catch_all.hpp>
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

TEST_CASE("AES basic encrypt/decrypt", "[aes_crypt_basic]"){
    OpenSSL_AES_Keys_st* keys = OpenSSL::aes_key_generation();
    REQUIRE(keys != nullptr);

    std::string message = "crypto";

    std::string enc = OpenSSL::aes_encrypt(message, keys->key, keys->iv);

    REQUIRE(!enc.empty());

    std::string dec = OpenSSL::aes_decrypt(enc, keys->key, keys->iv);

    REQUIRE(message == dec);

    delete keys;
}

TEST_CASE("AES buffer crypt", "[aes_buffer_crypt]"){
    OpenSSL_AES_Keys_st* keys = OpenSSL::aes_key_generation();
    REQUIRE(keys != nullptr);

    std::string message = OpenSSL_Utils::generateRandomString(1024);

    std::string enc = OpenSSL::aes_encrypt(message, keys->key, keys->iv);

    REQUIRE(!enc.empty());

    std::string dec = OpenSSL::aes_decrypt(enc, keys->key, keys->iv);

    REQUIRE(message == dec);
    REQUIRE(dec.size() == 1024);

    char charArray[1024];
    std::strncpy(charArray, dec.c_str(), sizeof(charArray) - 1);
    charArray[sizeof(charArray) - 1] = '\0';

    std::string rewind;
    rewind.resize(1024);

    for(int i=0;i<1024;i++){
        rewind[i] = charArray[i];
    }

    REQUIRE(dec == rewind);

    delete keys;
}