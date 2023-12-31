//
// Created by tombe on 31/12/2023.
//

#ifndef SECTRANS_OPENSSL_AES_H
#define SECTRANS_OPENSSL_AES_H

#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

struct OpenSSL_AES_Keys_st{
    std::string key;
    std::string iv;
};

typedef struct OpenSSL_AES_Keys_st OpenSSL_AES_Keys;

#endif //SECTRANS_OPENSSL_AES_H
