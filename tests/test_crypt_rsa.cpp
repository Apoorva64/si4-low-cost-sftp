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
    REQUIRE(skey != NULL);
    return skey;
}

std::string RSA_private_key(EVP_PKEY * key){
    char *skey = OpenSSL_Utils::get_rsa_private_key_str(key);
    REQUIRE(skey != NULL);
    return skey;
}

TEST_CASE("String Case", "[str_case]"){
    std::string alpha = "alpha";
    const char *s = alpha.c_str();

    REQUIRE(s[alpha.size()] == '\0');

    const char *d = alpha.data();

    REQUIRE(d[alpha.size()] == '\0');
}

TEST_CASE( "RSA key Generation", "[rsa_key]" ) {
    EVP_PKEY *key = RSA_generation();
    REQUIRE( key != nullptr);
    REQUIRE(!RSA_private_key(key).empty());
    REQUIRE(!RSA_public_key(key).empty());

    EVP_PKEY_free(key);
}

TEST_CASE("RSA key from value", "[rsa_from_str]"){
    std::string privKey = "-----BEGIN PRIVATE KEY-----\n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\n2r/yOuirUuOFt7YTet6h+oTpvsGk7hk=\n-----END PRIVATE KEY-----\n";
    std::string pubKey = "-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAtp8iFX04ycFJ/0o75aW1\nq28Yoaa1dc6UsQ/qtSxqqsex9cy/6TVVLlReDeRTb35A1HWkXI+9mNExHR4+tzu3\nwJkvA9bhzH9bDewCr85T77sv3SR2L3HqBc2CoUaAgasn5uv7ExYnh8Ymoa9vh4dn\nmi4eG8ySdMU0E5jWmyByJ5xjEiwz8YtX9M0wHVDs1cLWF1fp2hCSp3BS2gQB/R7Q\n0OixaFm942CTpqmbCjJkUK87bf9nnl9YGNNkKhsBek8bPQDo5lSSQ/1TvQFcvsTK\nXYOrVMeorQSiWAUaEoGx1MMCSLdOX41WY0LtLi9hhxrc8eFPja9kZ2wUD9MePC48\nzokT87waZnt7kElOWdq3RfKXHa+JH5d9R196ig/JhpAFVVpjhV8EaxkG6qJAov7H\nG+n4M8gKPEhAq3quSp0rpb34/hhy7ZHoKpzwMqTa2wmGVJJyqcZkdW5VlJbykMhd\nrgTYYbpKOg34USlwDUxpihUo5fOLgB9rOSnEUKwKIRbnmKdFJKLzs5f9uf2AY3PE\nY6EsUYhL+/1Y2PiBRlgaxfyj5gQ4EzE2yqi8rwDHmdAN7Aq7WSN9wV/HA9ebyEtG\n4A9SVm8aBqHmc0csuNiFUzrUGV49rytJT3Aau+I+rdTWVxNWomNl0ghZJOkErTTh\noz8G634H2Uer16T0hJ9GTvECAwEAAQ==\n-----END PUBLIC KEY-----\n";

    EVP_PKEY  *key2 = OpenSSL_Utils::get_key_from_str(pubKey, privKey);

    REQUIRE( key2 != nullptr);

    std::string privKey2 = RSA_private_key(key2);
    std::string pubKey2 = RSA_public_key(key2);

    REQUIRE(privKey == privKey2);
    REQUIRE(pubKey == pubKey2);

    EVP_PKEY_free(key2);
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

TEST_CASE("RSA encryption with only public key", "[rsa_enc_only_pub_key]"){
    EVP_PKEY *key = RSA_generation();
    REQUIRE( key != nullptr);

    std::string message = "cyber";

    std::string pubKey = RSA_public_key(key);

    EVP_PKEY  *key2 = OpenSSL_Utils::get_key_from_str(pubKey, "");
    REQUIRE(key2 != nullptr);

    std::string enc = OpenSSL::rsa_encrypt(key, message);
    std::string enc2 = OpenSSL::rsa_encrypt(key2, message);

    REQUIRE(!enc.empty());
    REQUIRE(!enc2.empty());

    std::string dec = OpenSSL::rsa_decrypt(key, enc);
    std::string dec2 = OpenSSL::rsa_decrypt(key, enc2);

    REQUIRE(dec == dec2);
    REQUIRE(dec == message);

    EVP_PKEY_free(key);
}