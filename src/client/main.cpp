#include <iostream>
#include "../common/SocketCommunication/SocketCommunication.h"
#include "../common/CLI11.hpp"

int main(int argc, char **argv) {
    SocketCommunication socketCommunication = SocketCommunication(
            8081,
            8080
    );

    socketCommunication.start();
    socketCommunication.test();
}