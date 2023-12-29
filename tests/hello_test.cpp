#define CONFIG_CATCH_MAIN
#include <catch2/catch_all.hpp>
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

EVP_PKEY *  RSA_generation(){
    printf("Start Generation...");
    EVP_PKEY * key = OpenSSL::rsa_key_generation();
    printf("OK\n");
   return key;
}

std::string RSA_public_key(EVP_PKEY * key){
    char *skey = OpenSSL_Utils::get_rsa_public_key_str(key);
    printf("A %s A\n", skey);
    return skey;
}

std::string RSA_private_key(EVP_PKEY * key){
    char *skey = OpenSSL_Utils::get_rsa_private_key_str(key);
    printf("A %s A\n", skey);
    return skey;
}

TEST_CASE( "RSA key Generation", "[rsa_key]" ) {
    EVP_PKEY *key = RSA_generation();
    REQUIRE( key != nullptr);
    REQUIRE(!RSA_private_key(key).empty());
    REQUIRE(!RSA_public_key(key).empty());

    EVP_PKEY_free(key);
}

TEST_CASE( "RSA key Generation with reverse", "[rsa_key_reverse]" ) {
    EVP_PKEY *key = RSA_generation();
    REQUIRE( key != nullptr);

    std::string privKey = RSA_private_key(key);
    std::string pubKey = RSA_public_key(key);

    EVP_PKEY  *key2 = OpenSSL_Utils::get_key_from_str(pubKey, privKey);

    REQUIRE( key2 != nullptr);

    std::string privKey2 = RSA_private_key(key2);
    std::string pubKey2 = RSA_public_key(key2);

    REQUIRE(privKey == privKey2);
    REQUIRE(pubKey == pubKey2);

    EVP_PKEY_free(key);
    EVP_PKEY_free(key2);
}

TEST_CASE("RSA Encryption/Decryption", "[rsa_enc_dec]"){
    EVP_PKEY *key = RSA_generation();
    REQUIRE( key != nullptr);

    std::string message = "cyber";

    std::string enc = OpenSSL::rsa_encrypt(key, message);

    REQUIRE(!enc.empty());
    REQUIRE(enc != message);

    std::string dec = OpenSSL::rsa_decrypt(key,enc);

    REQUIRE(message == dec);

    EVP_PKEY_free(key);
}