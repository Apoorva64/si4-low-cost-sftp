#include <iostream>
#include "../common/SocketCommunication/SocketCommunication.h"
#include "CLI11.hpp"
#include "client/Client.h"

int main(int argc, char **argv) {
    Client client = Client(
            8081,
            8080,
            argc,
            argv
    );

    OpenSSL_AES_Keys aesKeys;

    aesKeys.key = "p6Ix*(L/6NP)28HZ}_KQ25h@dWD+xB{^";
    aesKeys.iv = "a7fe8fed9f4v8e5d";

//    client.login();
//
//    client.upload(
//            "test.txt",
//            aesKeys,
//            "cGV3cGV3cGV3",
//            client.accessToken);
//    client.download(
//            "test.txt",
//            aesKeys, client.accessToken);
    return 0;
}