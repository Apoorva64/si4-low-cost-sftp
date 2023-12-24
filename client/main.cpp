#include <iostream>
#include "../common/SocketCommunication/SocketCommunication.h"
#include "CLI11.hpp"
#include "client/Client.h"

int main(int argc, char **argv) {
//    SocketCommunication socketCommunication = SocketCommunication(
//            8081,
//            8080
//    );
//
//    socketCommunication.start();
//    socketCommunication.test();
//    socketCommunication.send("upload|test.txt|cGV3cGV3cGV3");
//    socketCommunication.send("list");
//    std::cout << socketCommunication.receiveString() << std::endl;
//    socketCommunication.send("download|test.txt");
//    std::cout << socketCommunication.receiveString() << std::endl;

    Client client = Client(
            8081,
            8080,
            argc,
            argv
    );

    OpenSSL_AES_Keys aesKeys;

    aesKeys.key = "p6Ix*(L/6NP)28HZ}_KQ25h@dWD+xB{^";
    aesKeys.iv = "a7fe8fed9f4v8e5d";


    client.upload(
            "test.txt",
            aesKeys,
            "cGV3cGV3cGV3"
            );
    client.download(
            "test.txt",
            aesKeys
            );
    return 0;
}