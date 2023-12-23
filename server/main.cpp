#include <iostream>
#include "server/Server.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto server = Server(
            8080,
            8081);

    server.start();
    server.run();
}
