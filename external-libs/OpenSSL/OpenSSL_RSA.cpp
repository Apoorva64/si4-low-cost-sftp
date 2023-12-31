//
// Created by tombe on 29/12/2023.
//
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

EVP_PKEY * OpenSSL::rsa_key_generation() {
    EVP_PKEY_CTX * kctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    EVP_PKEY *pkey = nullptr;

    if(!kctx)
        throw std::runtime_error("Failed to get RSA context !");

    int code = EVP_PKEY_keygen_init(kctx);

    if(code <= 0)
        throw std::runtime_error("Failed to init key generation !");

    code = EVP_PKEY_CTX_set_rsa_keygen_bits(kctx, 4096);

    if(code <= 0)
        throw std::runtime_error("Failed to set RSA key length !");

    code = EVP_PKEY_keygen(kctx, &pkey);

    if(code <= 0)
        throw std::runtime_error("Failed to generate key !");

    EVP_PKEY_CTX_free(kctx);

    return pkey;
}

std::string OpenSSL::rsa_encrypt(EVP_PKEY *key, const std::string &message) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);
    unsigned char *out, *in;
    size_t inLen, outLen;

    in = OpenSSL_Utils::convert_string_to_uchar(message);
    inLen = message.size();

    if(!ctx)
        throw std::runtime_error("Failed to create encryption context !");

    if (EVP_PKEY_encrypt_init(ctx) <= 0)
        throw std::runtime_error("Failed to init encryption context !");

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
        throw std::runtime_error("Failed to add RSA padding !");

    if (EVP_PKEY_encrypt(ctx, NULL, &outLen, in, inLen) <= 0)
        throw std::runtime_error("Failed  get size of encrypt !");

    out = new unsigned char [outLen];

    if (EVP_PKEY_encrypt(ctx, out, &outLen, in, inLen) <= 0) {
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());
    }

    std::string result = OpenSSL_Utils::convert_uchar_to_string(out, outLen);

    EVP_PKEY_CTX_free(ctx);
    delete out;
    return result;
}

std::string OpenSSL::rsa_decrypt(EVP_PKEY *key, const std::string &message) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);
    unsigned char *out, *in;
    size_t inLen, outLen;

    in = OpenSSL_Utils::convert_string_to_uchar(message);
    inLen = message.size();

    if(!ctx)
        throw std::runtime_error("Failed to create encryption context !");

    if (EVP_PKEY_decrypt_init(ctx) <= 0)
        throw std::runtime_error("Failed to init encryption context !");

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
        throw std::runtime_error("Failed to add RSA padding !");

    if (EVP_PKEY_decrypt(ctx, NULL, &outLen, in, inLen) <= 0)
        throw std::runtime_error("Failed  get size of encrypt !");

    out = new unsigned char [outLen];

    if (EVP_PKEY_decrypt(ctx, out, &outLen, in, inLen) <= 0)
        throw std::runtime_error("Failed to encrypt !");

    std::string result = OpenSSL_Utils::convert_uchar_to_string(out, outLen);

    EVP_PKEY_CTX_free(ctx);
    delete out;
    return result;
}