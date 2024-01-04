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

/**
 * @struct OpenSSL_AES_Keys_st
 * @brief This structure represents a set of AES keys.
 *
 * The OpenSSL_AES_Keys_st structure contains the key and initialization vector (IV) for AES encryption and decryption.
 */
struct OpenSSL_AES_Keys_st{
    std::string key; ///< The AES key.
    std::string iv; ///< The initialization vector (IV) for AES.
};

typedef struct OpenSSL_AES_Keys_st OpenSSL_AES_Keys; ///< Defines OpenSSL_AES_Keys as an alias for struct OpenSSL_AES_Keys_st.

#endif //SECTRANS_OPENSSL_AES_H