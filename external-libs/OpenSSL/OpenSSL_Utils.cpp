//
// Created by tombe on 23/12/2023.
//

#include <openssl/rand.h>
#include "OpenSSL_Utils.h"
#include "OpenSSL.h"

/**
 * @brief Retrieves the latest OpenSSL error message.
 *
 * Fetches the error message from the OpenSSL error queue and returns it as a string.
 *
 * @return std::string The error message from the OpenSSL error queue.
 */
std::string OpenSSL_Utils::getOpenSSLError() {
    BIO *bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char *buf;
    size_t len = BIO_get_mem_data(bio, &buf);
    std::string ret(buf, len);
    BIO_free(bio);
    return ret;
}

/**
 * @brief Converts a string to an unsigned char array.
 *
 * Allocates memory for an unsigned char array and copies the contents of the string into it.
 *
 * @param text The string to be converted.
 * @return unsigned char* A pointer to the converted unsigned char array.
 */
unsigned char *OpenSSL_Utils::convert_string_to_uchar(const std::string &text) {
    unsigned char *text_convert = new unsigned char[text.size()];

    std::copy(text.begin(), text.end(), text_convert);
    text_convert[text.length()] = 0;
    return text_convert;
}

/**
 * @brief Converts a string to a vector of bytes.
 *
 * Converts the input string into a vector of bytes (uint8_t).
 *
 * @param message The string to be converted.
 * @return std::vector<uint8_t> The vector of bytes representing the string.
 */
std::vector<uint8_t> OpenSSL_Utils::str_to_bytes(const std::string &message) {
    std::vector<uint8_t> out(message.size());
    for (size_t n = 0; n < message.size(); n++) {
        out[n] = message[n];
    }
    return out;
}


/**
 * @brief Converts a vector of bytes to a string.
 *
 * Converts the input vector of bytes (uint8_t) into a string.
 *
 * @param bytes The vector of bytes to be converted.
 * @return std::string The string representation of the byte vector.
 */
std::string OpenSSL_Utils::bytes_to_str(const std::vector<uint8_t> &bytes) {
    return std::string(bytes.begin(), bytes.end());
}

/**
 * @brief Converts an unsigned char array to a vector of bytes.
 *
 * Converts the input unsigned char array into a vector of bytes (uint8_t).
 *
 * @param message The unsigned char array to be converted.
 * @param inLen The length of the unsigned char array.
 * @return std::vector<uint8_t> The vector of bytes representing the unsigned char array.
 */
std::vector<uint8_t> OpenSSL_Utils::uchar_to_bytes(unsigned char *message, unsigned int inLen) {
    std::vector<uint8_t> out(inLen);
    for (size_t n = 0; n < inLen; n++) {
        out[n] = message[n];
    }

    return out;
}


/**
 * @brief Retrieves the private key as a string from an EVP_PKEY object.
 *
 * Extracts the private key from the provided EVP_PKEY object and returns it as a PEM-formatted string.
 *
 * @param _pkey The EVP_PKEY object containing the private key.
 * @return char* The PEM-formatted private key as a string.
 * @throws std::runtime_error If the private key extraction fails.
 */
char *OpenSSL_Utils::get_rsa_private_key_str(EVP_PKEY *_pkey) {
    char *_rsaPrivateKeyStr = nullptr;
    BIO *bioPrivate = BIO_new(BIO_s_mem());

    if (!PEM_write_bio_PrivateKey(bioPrivate, _pkey, nullptr, nullptr, 0, nullptr, nullptr)) {
        throw std::runtime_error(getOpenSSLError());
    }

    // Obtenez la taille de la chaîne
    size_t length = BIO_pending(bioPrivate);

    // Allouez suffisamment d'espace, y compris pour le caractère nul de fin de chaîne
    _rsaPrivateKeyStr = new char[length + 1];

    // Lire les données dans la chaîne allouée
    BIO_read(bioPrivate, _rsaPrivateKeyStr, length);

    // Ajouter le caractère nul de fin de chaîne
    _rsaPrivateKeyStr[length] = '\0';

    // Nettoyez la ressource BIO
    BIO_free(bioPrivate);

    return _rsaPrivateKeyStr;
}

/**
 * @brief Retrieves the public key as a string from an EVP_PKEY object.
 *
 * Extracts the public key from the provided EVP_PKEY object and returns it as a PEM-formatted string.
 *
 * @param _pkey The EVP_PKEY object containing the public key.
 * @return char* The PEM-formatted public key as a string.
 * @throws std::runtime_error If the public key extraction fails.
 */
char *OpenSSL_Utils::get_rsa_public_key_str(EVP_PKEY *_pkey) {
    char *_rsaPrivateKeyStr = nullptr;
    BIO *bioPrivate = BIO_new(BIO_s_mem());

    if (!PEM_write_bio_PUBKEY(bioPrivate, _pkey)) {
        throw std::runtime_error(getOpenSSLError());
    }

    // Obtenez la taille de la chaîne
    size_t length = BIO_pending(bioPrivate);

    // Allouez suffisamment d'espace, y compris pour le caractère nul de fin de chaîne
    _rsaPrivateKeyStr = new char[length + 1];

    // Lire les données dans la chaîne allouée
    BIO_read(bioPrivate, _rsaPrivateKeyStr, length);

    // Ajouter le caractère nul de fin de chaîne
    _rsaPrivateKeyStr[length] = '\0';

    // Nettoyez la ressource BIO
    BIO_free(bioPrivate);

    return _rsaPrivateKeyStr;
}

/**
 * @brief Creates an EVP_PKEY object from public or private key strings.
 *
 * Generates an EVP_PKEY object using the provided public or private key strings.
 * At least one of the key strings must be non-empty.
 *
 * @param pubKey The public key as a string.
 * @param privKey The private key as a string.
 * @return EVP_PKEY* The generated EVP_PKEY object.
 * @throws std::runtime_error If both key strings are empty or if key creation fails.
 */
EVP_PKEY *OpenSSL_Utils::get_key_from_str(const std::string &pubKey, const std::string &privKey) {
    if (pubKey.empty() && privKey.empty()) {
        throw std::runtime_error("Key empty !");
    }
    EVP_PKEY *pkey = EVP_PKEY_new();

    if (!pubKey.empty()) {
        BIO *bioPublicKey = BIO_new_mem_buf(pubKey.c_str(), -1);
        PEM_read_bio_PUBKEY(bioPublicKey, &pkey, nullptr, nullptr);
        if (pkey == nullptr) {
            EVP_PKEY_free(pkey);
            throw std::runtime_error("Public Key set Error");
        }
    }

    if (!privKey.empty()) {
        BIO *bioPrivateKey = BIO_new_mem_buf(privKey.c_str(), -1);
        PEM_read_bio_PrivateKey(bioPrivateKey, &pkey, nullptr, nullptr);
        if (pkey == nullptr) {
            EVP_PKEY_free(pkey);
            throw std::runtime_error("Private Key set Error");
        }
    }

    return pkey;
}

/**
 * @brief Converts an unsigned char array to a string.
 *
 * Converts the input unsigned char array into a string.
 *
 * @param message The unsigned char array to be converted.
 * @param inLen The length of the unsigned char array.
 * @return std::string The string representation of the unsigned char array.
 */
std::string OpenSSL_Utils::convert_uchar_to_string(unsigned char *message, unsigned int inLen) {
    return OpenSSL_Utils::bytes_to_str(OpenSSL_Utils::uchar_to_bytes(message, inLen));
}

/**
 * @brief Generates a random string of a specified size.
 *
 * Creates a random string of the specified size using secure random number generation.
 *
 * @param size The size of the random string to generate.
 * @return std::string The generated random string.
 * @throws std::runtime_error If the random number generation fails.
 */
std::string OpenSSL_Utils::generateRandomString(int size) {
    auto *buf = new unsigned char[size];

    if (!RAND_bytes(buf, size)) {
        throw std::runtime_error(OpenSSL_Utils::getOpenSSLError());
    }

    std::string res = OpenSSL_Utils::convert_uchar_to_string(buf, size);

    delete[] buf;
    return res;
}

/**
 * @brief Creates an OpenSSL_AES_Keys_st object from key and IV strings.
 *
 * Generates an OpenSSL_AES_Keys_st object using the provided key and IV strings.
 * At least one of the strings must be non-empty.
 *
 * @param key The AES key as a string.
 * @param iv The AES IV as a string.
 * @return OpenSSL_AES_Keys_st* The generated OpenSSL_AES_Keys_st object.
 * @throws std::runtime_error If both key and IV strings are empty.
 */
OpenSSL_AES_Keys_st *OpenSSL_Utils::get_aes_key_from_str(const std::string &key, const std::string &iv) {
    if (key.empty() && iv.empty()) {
        throw std::runtime_error("Key Error");
    }

    auto *keys = new OpenSSL_AES_Keys_st;

    keys->key = key;
    keys->iv = iv;

    return keys;
}

/**
 * @brief Converts a char array to a string.
 *
 * Converts the input char array into a string.
 *
 * @param message The char array to be converted.
 * @return std::string The string representation of the char array.
 */
std::string OpenSSL_Utils::convert_char_to_string(char *message) {
    return std::string(message);
}
