#include <iostream>
#include "server/Server.h"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug);
    auto server = Server(
            8080,
            8081);
    server.parse(argc, argv);
}
