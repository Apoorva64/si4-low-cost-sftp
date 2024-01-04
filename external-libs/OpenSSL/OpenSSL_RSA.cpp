//
// Created by tombe on 29/12/2023.
//
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

/**
 * @brief Generates an RSA key pair.
 *
 * Creates a new RSA key pair with a specified key size using OpenSSL's EVP functions.
 * Returns a pointer to the generated key pair.
 *
 * @return EVP_PKEY* A pointer to the generated RSA key pair.
 * @throws std::runtime_error If any step of the key generation process fails.
 */
EVP_PKEY * OpenSSL::rsa_key_generation() {
    EVP_PKEY_CTX * kctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    EVP_PKEY *pkey = nullptr;

    if(!kctx)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    int code = EVP_PKEY_keygen_init(kctx);

    if(code <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    code = EVP_PKEY_CTX_set_rsa_keygen_bits(kctx, 4096);

    if(code <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    code = EVP_PKEY_keygen(kctx, &pkey);

    if(code <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    EVP_PKEY_CTX_free(kctx);

    return pkey;
}

/**
 * @brief Encrypts a message using RSA and OAEP padding.
 *
 * Encrypts the provided message using the specified RSA public key and OAEP padding.
 * Returns the encrypted message as a string.
 *
 * @param key The RSA public key for encryption.
 * @param message The plaintext message to encrypt.
 * @return std::string The encrypted message as a string.
 * @throws std::runtime_error If any step of the encryption process fails.
 */
std::string OpenSSL::rsa_encrypt(EVP_PKEY *key, const std::string &message) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);
    unsigned char *out, *in;
    size_t inLen, outLen;

    in = OpenSSL_Utils::convert_string_to_uchar(message);
    inLen = message.size();

    if(!ctx)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    if (EVP_PKEY_encrypt_init(ctx) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    if (EVP_PKEY_encrypt(ctx, NULL, &outLen, in, inLen) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    out = new unsigned char [outLen];

    if (EVP_PKEY_encrypt(ctx, out, &outLen, in, inLen) <= 0) {
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());
    }

    std::string result = OpenSSL_Utils::convert_uchar_to_string(out, outLen);

    EVP_PKEY_CTX_free(ctx);
    delete out;
    return result;
}

/**
 * @brief Decrypts a message using RSA and OAEP padding.
 *
 * Decrypts the provided message using the specified RSA private key and OAEP padding.
 * Returns the decrypted message as a string.
 *
 * @param key The RSA private key for decryption.
 * @param message The encrypted message to decrypt.
 * @return std::string The decrypted message as a string.
 * @throws std::runtime_error If any step of the decryption process fails.
 */
std::string OpenSSL::rsa_decrypt(EVP_PKEY *key, const std::string &message) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);
    unsigned char *out, *in;
    size_t inLen, outLen;

    in = OpenSSL_Utils::convert_string_to_uchar(message);
    inLen = message.size();

    if(!ctx)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    if (EVP_PKEY_decrypt_init(ctx) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    if (EVP_PKEY_decrypt(ctx, NULL, &outLen, in, inLen) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    out = new unsigned char [outLen];

    if (EVP_PKEY_decrypt(ctx, out, &outLen, in, inLen) <= 0)
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());

    std::string result = OpenSSL_Utils::convert_uchar_to_string(out, outLen);

    EVP_PKEY_CTX_free(ctx);
    delete out;
    return result;
}