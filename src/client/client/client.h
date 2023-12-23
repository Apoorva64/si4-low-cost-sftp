//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_CLIENT_H
#define SECTRANS_CLIENT_H


#include "../../common/CLI11.hpp"

class client : public CLI::App {

    client(int argc, char **argv);
    std::string filename;
};


#endif //SECTRANS_CLIENT_H
