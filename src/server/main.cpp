#include <iostream>
#include "../common/SocketCommunication/SocketCommunication.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    SocketCommunication socketCommunication = SocketCommunication(
            8080,
            8081
            );

    socketCommunication.start();
    socketCommunication.run();
}
