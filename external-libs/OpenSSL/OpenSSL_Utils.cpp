//
// Created by tombe on 23/12/2023.
//

#include "OpenSSL_Utils.h"

std::string getOpenSSLError()
{
    BIO *bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char *buf;
    size_t len = BIO_get_mem_data(bio, &buf);
    std::string ret(buf, len);
    BIO_free(bio);
    return ret;
}

unsigned char* OpenSSL_Utils::convert_string_to_uchar(const std::string &text) {
    unsigned char *text_convert = new unsigned char [text.size()];

    std::copy( text.begin(), text.end(), text_convert );
    text_convert[text.length()] = 0;
    return text_convert;
}

std::vector<uint8_t> OpenSSL_Utils::str_to_bytes(const std::string& message) {
    std::vector<uint8_t> out(message.size());
    for(size_t n = 0; n < message.size(); n++) {
        out[n] = message[n];
    }
    return out;
}

std::string OpenSSL_Utils::bytes_to_str(const std::vector<uint8_t>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

std::vector<uint8_t> OpenSSL_Utils::uchar_to_bytes(unsigned char* message, unsigned int inLen){
    std::vector<uint8_t> out(inLen);
    for(size_t n = 0; n < inLen; n++){
        out[n] = message[n];
    }

    return out;
}

char* OpenSSL_Utils::get_rsa_private_key_str(EVP_PKEY* _pkey)
{
    char* _rsaPrivateKeyStr = nullptr;
    BIO* bioPrivate = BIO_new(BIO_s_mem());

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

char* OpenSSL_Utils::get_rsa_public_key_str(EVP_PKEY* _pkey)
{
    char * _rsaPrivateKeyStr = nullptr;
    BIO *bioPrivate = BIO_new(BIO_s_mem());

    if (!PEM_write_bio_PUBKEY(bioPrivate, _pkey)){
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

EVP_PKEY* OpenSSL_Utils::get_key_from_str(const std::string& pubKey, const std::string& privKey){
    EVP_PKEY *pkey = EVP_PKEY_new();

    if(!pubKey.empty()) {
        BIO* bioPublicKey = BIO_new_mem_buf(pubKey.c_str(), -1);
        PEM_read_bio_PUBKEY(bioPublicKey, &pkey, nullptr, nullptr);
        if (pkey == nullptr) {
            EVP_PKEY_free(pkey);
            throw std::runtime_error("Public Key set Error");
        }
    }

    if(!privKey.empty()) {
        BIO* bioPrivateKey = BIO_new_mem_buf(privKey.c_str(), -1);
        PEM_read_bio_PrivateKey(bioPrivateKey, &pkey, nullptr, nullptr);
        if (pkey == nullptr) {
            EVP_PKEY_free(pkey);
            throw std::runtime_error("Private Key set Error");
        }
    }

    return pkey;
}

std::string OpenSSL_Utils::convert_uchar_to_string(unsigned char *message, unsigned int inLen) {
    return OpenSSL_Utils::bytes_to_str(OpenSSL_Utils::uchar_to_bytes(message, inLen));
}
