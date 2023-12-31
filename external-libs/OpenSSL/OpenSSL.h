/*
 * Copyright (c) 2023 Remy van Elst
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once


#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>

#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

#include "OpenSSL_AES.h"

struct OpenSSLFree
{
    void operator() (BIO* bio) const
    { BIO_free(bio); }

    void operator() (X509* x509) const
    { X509_free(x509); }

    /**
     * This frees the STACK_OF and
     * all the items in it.
     */
    void operator() (STACK_OF(X509)* st) const
    {
        for(int i = 0; i < sk_X509_num(st); ++i)
            X509_free(sk_X509_value(st, i));
        sk_X509_free(st);
    }

    void operator() (X509_STORE* store) const
    { X509_STORE_free(store); }

    void operator() (X509_STORE_CTX* ctx) const
    { X509_STORE_CTX_free(ctx); }

    void operator() (X509_VERIFY_PARAM* param) const
    { X509_VERIFY_PARAM_free(param); }

    void operator() (GENERAL_NAME* gn) const
    {GENERAL_NAME_free(gn); }

    /**
     * This frees the STACK_OF and
     * all the items in it.
     */
    void operator() (STACK_OF(GENERAL_NAME)* st) const
    {
        for(int i = 0; i < sk_GENERAL_NAME_num(st); ++i)
            GENERAL_NAME_free(sk_GENERAL_NAME_value(st, i));
        sk_GENERAL_NAME_free(st);
    }

    void operator() (EVP_PKEY* evp_pkey) const
    { EVP_PKEY_free(evp_pkey); }

    void operator() (EVP_MD_CTX* evp_md_ctx) const
    { EVP_MD_CTX_free(evp_md_ctx); }
};

using X509_uptr = std::unique_ptr<X509, OpenSSLFree>;
using STACK_OF_X509_uptr = std::unique_ptr<STACK_OF(X509), OpenSSLFree>;
using BIO_MEM_uptr = std::unique_ptr<BIO, OpenSSLFree>;
using EVP_PKEY_uptr = std::unique_ptr<EVP_PKEY, OpenSSLFree>;
using BUF_MEM_uptr = std::unique_ptr<BUF_MEM,OpenSSLFree>;
using X509_STORE_CTX_uptr = std::unique_ptr<X509_STORE_CTX, OpenSSLFree>;
using X509_STORE_uptr = std::unique_ptr<X509_STORE, OpenSSLFree>;
using X509_VERIFY_PARAM_uptr = std::unique_ptr<X509_VERIFY_PARAM, OpenSSLFree>;
using GENERAL_NAME_uptr = std::unique_ptr<GENERAL_NAME, OpenSSLFree>;
using STACK_OF_GENERAL_NAME_uptr = std::unique_ptr<STACK_OF(GENERAL_NAME), OpenSSLFree>;
using EVP_MD_CTX_uptr = std::unique_ptr<EVP_MD_CTX, OpenSSLFree>;

inline static const int maxKeySize = 4096;

class OpenSSL {
public:
    /**
     * Convenience wrappers
     */
    [[nodiscard]] static int verify_cert_signed_by_chain(const std::string& cert_pem,
                                                         const std::string& issuer_pem);

    [[nodiscard]] static int verify_cert_signed_by_chain(const std::string& cert_pem,
                                                         const std::string& issuer_pem,
                                                         int (*verify_cb)(int, X509_STORE_CTX *));

    [[nodiscard]] static int verify_cert_signed_by_chain(const std::string& cert_pem,
                                                         const std::string& issuer_pem,
                                                         const X509_VERIFY_PARAM* x509_verify_param);

    /**
     * Verifies if a certificate is signed by a chain.
     * @param cert_pem single PEM encoded certificate to check against chain
     * @param chain PEM encoded chain (list of certificates)
     * IMPORTANT: chain must be complete, ordered and ending in a self signed trusted root.
     * @param x509_verify_param optional X509_VERIFY_PARAM to for example disable time checks, X509_V_FLAG_NO_CHECK_TIME
     * @param verify_cb optional X509_STORE_CTX_verify_cb function
     * @return 1 if OK, 0 if NOT OK, -1 on error
     */
    [[nodiscard]] static int verify_cert_signed_by_chain(const std::string& cert_pem,
                                                         const std::string& issuer_pem,
                                                         const X509_VERIFY_PARAM* x509_verify_param,
                                                         int (*verify_cb)(int, X509_STORE_CTX *));

    /**
     * Uses OpenSSL X509_verify to verify a certificates signature
     * (eg. if it's signed by the issuer provided).
     * @param cert_pem single PEM encoded certificate to check against issuer
     * @param issuer_pem single PEM encoded issuer certificate
     * @return 1 if OK, 0 if NOT OK, -1 on error
     */
    [[nodiscard]] static int verify_cert_signed_by_issuer(const std::string& cert_pem,
                                                          const std::string& issuer_pem) ;


    /**
     * Returns a unique_ptr<X509>, requiring no manual X509_free
     * @param cert_pem PEM encoded certificate
     */
    [[nodiscard]] static X509_uptr cert_to_x509(const std::string& cert_pem) ;


    /**
     * @param x509 OpenSSL X509 struct filled with certificate.
     * @return Contents of issuer field of certificate, empty on error.
     */
    static std::string x509_issuer (const X509* x509) ;

    /**
     * @param x509 OpenSSL X509 struct filled with certificate.
     * @return Contents of subject field of certificate, empty on error.
     */
    static std::string x509_subject (const X509* x509);

    /**
     * Parses the X509* and returns the subjectAlternativeNames
     * @param x509 OpenSSL X509 struct filled with certificate
     * @return vector of strings filled with subjectAlternativeName
     * NOTE: only parses DNS:. Not IP: or others.
     */
    static std::vector<std::string> x509_subject_alternative_dns_names(const X509* x509);

    /**
     * @param cert_pem PEM encoded certificates
     */
    [[nodiscard]] static std::vector<X509_uptr> certs_to_x509(const std::string& certs_pem);

    /**
     * Returns a unique_ptr<STACK_OF(X509)>, requiring no manual X509_free
     * @param cert_pem PEM encoded certificates
     */
    [[nodiscard]] static STACK_OF_X509_uptr certs_to_stack_of_x509(const std::string& certs_pem) ;


    /**
     * Parses X509* and returns public key (if found), otherwise nullptr
     * @param x509 OpenSSL X509 struct filled with certificate.
     */
    [[nodiscard]] static EVP_PKEY_uptr x509_to_evp_pubkey(const X509* x509);


    /**
     * Returns the public key in PEM format if found, otherwise empty
     * @param x509 OpenSSL X509 struct filled with certificate.
     */
    [[nodiscard]] static std::string x509_to_public_key_pem(const X509* x509);


    /**
     * Verifies if a sha256 signed digest signed the provided message.
     * cli verify: openssl dgst -sha256 -verify  <(openssl x509 -in sign.crt  -pubkey -noout) -signature signature.bin message.txt
     * cli sign  : openssl dgst -sha256 -sign sign.key -out signature.bin message.txt
     * @param message original message that was signed
     * @param base64_encoded_signature binary signature data encoded as base64
     * @param x509_that_has_pubkey_that_signed_the_message certificate with publickey that signed the messag
     * @return 1 on verify correct, 0 on verify incorrect, -1 on error,
     */
    [[nodiscard]] static int verify_sha256_digest_signature(const std::string& message, const std::string& base64_encoded_signature, const X509* x509_that_has_pubkey_that_signed_the_message);

    /**
     * Uses OpenSSL to decode a base64 message string.
     */
    [[nodiscard]] static std::string base64_decode(const std::string& message);

    /**
   * Uses OpenSSL to encode a string to base64
   */
    [[nodiscard]] static std::string base64_encode(const std::string& message);

    [[nodiscard]] static OpenSSL_AES_Keys* aes_key_generation();
    [[nodiscard]] static std::string aes_encrypt(const std::string& message, const std::string& key, const std::string& iv);
    [[nodiscard]] static std::string aes_decrypt(const std::string& message, const std::string& key, const std::string& iv);
    [[nodiscard]] static EVP_PKEY * rsa_key_generation();
    [[nodiscard]] static std::string rsa_encrypt(EVP_PKEY *key, const std::string& message);
    [[nodiscard]] static std::string rsa_decrypt(EVP_PKEY *key, const std::string& message);

    static bool is_base64(const std::string &message);

private:

    /**
     * Used as a base class to convert subject or issuer to std::string,
     * handles allocation and conversion.
     * @param X509_X_NAME_FUNC Lambda that calls for example X509_get_subject_name.
     * @return
     */
    static std::string x509_name_base(const X509 *x509,
                                      const std::function<void(const X509 *,
                                                               const BIO_MEM_uptr &)> &X509_X_NAME_FUNC);


    static std::vector<char> read_binary_file(const std::string& filename);

    inline static const std::string allowed_base64{
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
            'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
            'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
            'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
            's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
            '3', '4', '5', '6', '7', '8', '9', '+', '/', '='};

    static std::string stripNonBase64FromString(const std::string &message);

};


