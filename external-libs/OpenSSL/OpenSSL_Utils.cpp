//
// Created by tombe on 23/12/2023.
//

#include "OpenSSL_Utils.h"


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
    char * _rsaPrivateKeyStr;
    BIO *bioPrivate = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(bioPrivate, _pkey, nullptr, nullptr, 0, nullptr, nullptr);

    BIO_flush(bioPrivate);
    BIO_get_mem_data(bioPrivate, &_rsaPrivateKeyStr);

    return _rsaPrivateKeyStr;
}

char* OpenSSL_Utils::get_rsa_public_key_str(EVP_PKEY* _pkey)
{
    char * _rsaPrivateKeyStr;
    BIO *bioPrivate = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(bioPrivate, _pkey, nullptr, nullptr, 0, nullptr, nullptr);

    BIO_flush(bioPrivate);
    BIO_get_mem_data(bioPrivate, &_rsaPrivateKeyStr);

    return _rsaPrivateKeyStr;
}

EVP_PKEY* OpenSSL_Utils::get_key_from_str(const std::string& pubKey, const std::string& privKey){
    BIO *bioPrivate = BIO_new(BIO_s_mem());
    BIO_write(bioPrivate, privKey.c_str(), static_cast<int>(privKey.length()));

    EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bioPrivate, nullptr, nullptr, nullptr);

    BIO_free(bioPrivate);

    bioPrivate = BIO_new(BIO_s_mem());
    BIO_write(bioPrivate, pubKey.c_str(), static_cast<int>(pubKey.length()));

    PEM_read_bio_PUBKEY(bioPrivate, &pkey, nullptr, nullptr);

    BIO_free(bioPrivate);

    return pkey;
}

std::string OpenSSL_Utils::convert_uchar_to_string(unsigned char *message, unsigned int inLen) {
    return OpenSSL_Utils::bytes_to_str(OpenSSL_Utils::uchar_to_bytes(message, inLen));
}
