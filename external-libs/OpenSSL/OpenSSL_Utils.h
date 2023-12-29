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

#pragma once


class OpenSSL_Utils {
public:
    [[nodiscard]] static unsigned char *convert_string_to_uchar(const std::string &text);
    [[nodiscard]] static std::string convert_uchar_to_string(unsigned char* message, unsigned int inLen);
    [[nodiscard]] static std::vector<uint8_t> str_to_bytes(const std::string& message);
    [[nodiscard]] static std::string bytes_to_str(const std::vector<uint8_t>& bytes);
    [[nodiscard]] static std::vector<uint8_t> uchar_to_bytes(unsigned char* message, unsigned int inLen);
    [[nodiscard]] static char* get_rsa_private_key_str(EVP_PKEY* _pkey);
    [[nodiscard]] static char* get_rsa_public_key_str(EVP_PKEY* _pkey);
    [[nodiscard]] static EVP_PKEY *get_key_from_str(const std::string& pubKey, const std::string& privKey);
};

