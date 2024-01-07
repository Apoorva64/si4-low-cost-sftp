//
// Created by XKHC2270 on 07/01/2024.
//

#ifndef SECTRANS_SERVERERROR_H
#define SECTRANS_SERVERERROR_H


#include <exception>
#include <string>
#include <stdexcept>

class PublicServerRuntimeError : public std::runtime_error {
public:
    explicit PublicServerRuntimeError(const std::string& msg):runtime_error(msg.c_str()){}
};

class PrivateServerRuntimeError : public std::runtime_error {
public:
    explicit PrivateServerRuntimeError(const std::string& msg): runtime_error(msg.c_str()){}
};


#endif //SECTRANS_SERVERERROR_H

