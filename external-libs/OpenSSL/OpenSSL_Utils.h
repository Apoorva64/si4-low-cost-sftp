//
// Created by tombe on 23/12/2023.
//

#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/aes.h>

#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

#include "OpenSSL_AES.h"

#pragma once

/**
 * @class OpenSSL_Utils
 * @brief This class provides utility functions for OpenSSL operations.
 *
 * The OpenSSL_Utils class provides functionalities for converting between different data types,
 * extracting keys from EVP_PKEY objects, generating random strings, and handling OpenSSL errors.
 */
class OpenSSL_Utils {
public:
    /**
     * @brief Convert a string to an unsigned char array.
     *
     * @param text The string to be converted.
     * @return A pointer to the converted unsigned char array.
     */
    [[nodiscard]] static unsigned char *convert_string_to_uchar(const std::string &text);

    /**
     * @brief Convert an unsigned char array to a string.
     *
     * @param message The unsigned char array to be converted.
     * @param inLen The length of the unsigned char array.
     * @return The converted string.
     */
    [[nodiscard]] static std::string convert_uchar_to_string(unsigned char* message, unsigned int inLen);

    /**
     * @brief Convert a char array to a string.
     *
     * @param message The char array to be converted.
     * @return The converted string.
     */
    [[nodiscard]] static std::string convert_char_to_string(char * message);

    /**
     * @brief Convert a string to a vector of bytes.
     *
     * @param message The string to be converted.
     * @return The converted vector of bytes.
     */
    [[nodiscard]] static std::vector<uint8_t> str_to_bytes(const std::string& message);

    /**
     * @brief Convert a vector of bytes to a string.
     *
     * @param bytes The vector of bytes to be converted.
     * @return The converted string.
     */
    [[nodiscard]] static std::string bytes_to_str(const std::vector<uint8_t>& bytes);

    /**
     * @brief Convert an unsigned char array to a vector of bytes.
     *
     * @param message The unsigned char array to be converted.
     * @param inLen The length of the unsigned char array.
     * @return The converted vector of bytes.
     */
    [[nodiscard]] static std::vector<uint8_t> uchar_to_bytes(unsigned char* message, unsigned int inLen);

    /**
     * @brief Retrieve the private key as a string from an EVP_PKEY object.
     *
     * @param _pkey The EVP_PKEY object containing the private key.
     * @return The PEM-formatted private key as a string.
     */
    [[nodiscard]] static char* get_rsa_private_key_str(EVP_PKEY* _pkey);

    /**
     * @brief Retrieve the public key as a string from an EVP_PKEY object.
     *
     * @param _pkey The EVP_PKEY object containing the public key.
     * @return The PEM-formatted public key as a string.
     */
    [[nodiscard]] static char* get_rsa_public_key_str(EVP_PKEY* _pkey);

    /**
     * @brief Create an EVP_PKEY object from public or private key strings.
     *
     * @param pubKey The public key as a string.
     * @param privKey The private key as a string.
     * @return The generated EVP_PKEY object.
     */
    [[nodiscard]] static EVP_PKEY *get_key_from_str(const std::string& pubKey, const std::string& privKey);

    /**
     * @brief Create an OpenSSL_AES_Keys_st object from key and IV strings.
     *
     * @param key The AES key as a string.
     * @param iv The AES IV as a string.
     * @return The generated OpenSSL_AES_Keys_st object.
     */
    [[nodiscard]] static OpenSSL_AES_Keys_st* get_aes_key_from_str(const std::string& key, const std::string& iv);

    /**
     * @brief Retrieve the latest OpenSSL error message.
     *
     * @return The error message from the OpenSSL error queue.
     */
    [[nodiscard]] static std::string getOpenSSLError();

    /**
     * @brief Generate a random string of a specified size.
     *
     * @param size The size of the random string to generate.
     * @return The generated random string.
     */
    [[nodiscard]] static std::string generateRandomString(int size);
};