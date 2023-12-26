#include <iostream>
#include "server/Server.h"
#include "spdlog/spdlog.h"

int main() {
    spdlog::set_level(spdlog::level::debug);
    auto server = Server(
            8080,
            8081);

    server.start();
//    server.createKeycloakResource("test.txt", "user1");
    server.run();

}
