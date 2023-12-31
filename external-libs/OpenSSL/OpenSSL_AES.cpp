//
// Created by tombe on 23/12/2023.
//
#include "OpenSSL.h"
#include "OpenSSL_Utils.h"

/**
 * @brief Encrypts a message using AES-256-CBC.
 *
 * Encrypts the provided message using the specified initialization vector (IV) and key.
 * The encrypted message is stored in the output vector.
 *
 * @param m_iv The initialization vector for encryption.
 * @param key The encryption key.
 * @param message The plaintext message to encrypt.
 * @param output The vector to store the encrypted message.
 */
void encrypt(const std::vector<uint8_t> &m_iv, const std::vector<uint8_t> &key, const std::vector<uint8_t> &message,
             std::vector<uint8_t> &output) {
    output.resize(message.size() * AES_BLOCK_SIZE);
    int inlen = message.size();
    int outlen = 0;
    size_t total_out = 0;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    EVP_CIPHER_CTX_init(ctx);

    const std::vector<uint8_t> enc_key = key;

    int res;
    res = EVP_EncryptInit(ctx, EVP_aes_256_cbc(), enc_key.data(), m_iv.data());
    res = EVP_EncryptUpdate(ctx, output.data(), &outlen, message.data(), inlen);
    total_out += outlen;
    res = EVP_EncryptFinal(ctx, output.data() + total_out, &outlen);
    total_out += outlen;

    output.resize(total_out);

    EVP_CIPHER_CTX_free(ctx);
}

/**
 * @brief Decrypts a message using AES-256-CBC.
 *
 * Decrypts the provided message using the specified initialization vector (IV) and key.
 * The decrypted message is stored in the output vector.
 *
 * @param m_iv The initialization vector for decryption.
 * @param key The decryption key.
 * @param message The encrypted message to decrypt.
 * @param output The vector to store the decrypted message.
 */
void decrypt(const std::vector<uint8_t> &m_iv, const std::vector<uint8_t> &key, const std::vector<uint8_t> &message,
             std::vector<uint8_t> &output) {
    output.resize(message.size() * 3);
    int outlen = 0;
    size_t total_out = 0;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    // means you have already 32 bytes keys
    const std::vector<uint8_t> enc_key = key;
    std::vector<uint8_t> target_message;
    std::vector<uint8_t> iv;

    iv = m_iv;
    target_message = message;

    int inlen = target_message.size();

    int res;
    res = EVP_DecryptInit(ctx, EVP_aes_256_cbc(), enc_key.data(), iv.data());
    res = EVP_DecryptUpdate(ctx, output.data(), &outlen, target_message.data(), inlen);
    total_out += outlen;
    res = EVP_DecryptFinal(ctx, output.data() + outlen, &outlen);
    total_out += outlen;

    output.resize(total_out);
    EVP_CIPHER_CTX_free(ctx);
}

/**
 * @brief Generates a new set of AES keys.
 *
 * Creates a new AES key and initialization vector using secure random number generation.
 * Returns a pointer to a structure containing the generated key and IV.
 *
 * @return OpenSSL_AES_Keys_st* A pointer to the structure containing the AES key and IV.
 * @throws std::runtime_error If the random number generation fails.
 */
OpenSSL_AES_Keys_st *OpenSSL::aes_key_generation() {
    auto *keys = new OpenSSL_AES_Keys_st;
    unsigned char key[32], iv[16];

    if (!RAND_bytes(key, 32)) {
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());
    }
    if (!RAND_bytes(iv, 16)) {
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());
    }

    keys->key = OpenSSL_Utils::convert_uchar_to_string(key, 32);
    keys->iv = OpenSSL_Utils::convert_uchar_to_string(iv, 16);

    return keys;
}

/**
 * @brief Encrypts a message using AES-256-CBC and returns the result as a string.
 *
 * Converts the input message, key, and IV to byte vectors, encrypts the message, and then
 * converts the encrypted byte vector back to a string.
 *
 * @param message The plaintext message to encrypt.
 * @param key The encryption key as a string.
 * @param iv The initialization vector as a string.
 * @return std::string The encrypted message as a string.
 */
std::string OpenSSL::aes_encrypt(const std::string &message, const std::string &key, const std::string &iv) {
    std::vector<uint8_t> enc_result;
    std::vector<uint8_t> message_array = OpenSSL_Utils::str_to_bytes(message);
    std::vector<uint8_t> key_array = OpenSSL_Utils::str_to_bytes(key);
    std::vector<uint8_t> iv_array = OpenSSL_Utils::str_to_bytes(iv);

    encrypt(iv_array, key_array, message_array, enc_result);

    return OpenSSL_Utils::bytes_to_str(enc_result);
}

/**
 * @brief Decrypts a message using AES-256-CBC and returns the result as a string.
 *
 * Converts the input message, key, and IV to byte vectors, decrypts the message, and then
 * converts the decrypted byte vector back to a string.
 *
 * @param message The encrypted message to decrypt.
 * @param key The decryption key as a string.
 * @param iv The initialization vector as a string.
 * @return std::string The decrypted message as a string.
 */
std::string OpenSSL::aes_decrypt(const std::string &message, const std::string &key, const std::string &iv) {
    std::vector<uint8_t> dec_result;
    std::vector<uint8_t> message_array = OpenSSL_Utils::str_to_bytes(message);
    std::vector<uint8_t> key_array = OpenSSL_Utils::str_to_bytes(key);
    std::vector<uint8_t> iv_array = OpenSSL_Utils::str_to_bytes(iv);

    decrypt(iv_array, key_array, message_array, dec_result);

    return OpenSSL_Utils::bytes_to_str(dec_result);
}

