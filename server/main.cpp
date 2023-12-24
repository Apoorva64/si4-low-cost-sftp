#include <iostream>
#include "server/Server.h"

int main() {
    auto server = Server(
            8080,
            8081);

    server.start();
    server.run();
}
