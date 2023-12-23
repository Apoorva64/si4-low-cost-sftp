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