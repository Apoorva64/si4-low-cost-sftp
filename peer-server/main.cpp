
#include <iostream>
#include "spdlog/spdlog.h"
#include "PeerServer/PeerServer.h"
#include "SessionStorage/SessionStorage.h"

SessionStorage *sessionStorage = nullptr;

int main() {
    spdlog::set_level(spdlog::level::debug);
    sessionStorage = new SessionStorage(9000, 9000 + std::thread::hardware_concurrency());
    auto peerServer = PeerServer(sessionStorage, 8080, 8081);
    peerServer.start();
    peerServer.run();
}