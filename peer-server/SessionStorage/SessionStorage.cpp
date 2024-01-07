//
// Created by tombe on 29/12/2023.
//

#include "SessionStorage.h"
#include "spdlog/sinks/stdout_color_sinks.h"

/**
 * @brief Constructs a SessionStorage object with a range of ports.
 *
 * Initializes the SessionStorage with a minimum and maximum port range, sets the current free port to the minimum port,
 * and allocates a list to store session information based on the range of ports provided.
 *
 * @param minPort The minimum port number available for sessions.
 * @param maxPort The maximum port number available for sessions.
 */
SessionStorage::SessionStorage(int minPort, int maxPort) : sessionList(maxPort - minPort), minPort(minPort),
                                                           maxPort(maxPort), currentFreePort(minPort) {
    logger = spdlog::stdout_color_mt("SessionStorage");
    logger->info("| SessionStorage.init | SessionStorage initialized with minPort: {}, maxPort: {}", minPort, maxPort);
}

/**
 * @brief Generates a new session for a client.
 *
 * Creates a new session with the next available port number, stores it in the session list, and returns the session ID.
 * Throws a runtime error if there are no more free ports available.
 *
 * @param clientPort The port number of the client.
 * @return int The session ID of the newly created session.
 * @throws std::runtime_error If there are no free ports available to create a new session.
 */
int SessionStorage::generateSession(int clientPort) {
    logger->info("| SessionStorage.generateSession | Generating session for client on port {}", clientPort);

    SESSION_CLIENT new_session;

    new_session.clientPort = clientPort;
    new_session.serverPort = getFreePort();


    this->sessionList.push_back(new_session);

    return new_session.serverPort;
}

/**
 * @brief Retrieves a session by its ID.
 *
 * Returns the session information for the given session ID.
 *
 * @param id The session ID.
 * @return SESSION_CLIENT The session information corresponding to the session ID.
 */
SESSION_CLIENT SessionStorage::getSession(int id) {
    for(auto const & i : this->sessionList){
        if(i.serverPort == id){
            return i;
        }
    }
    throw std::runtime_error("Session not found !");
}

/**
 * @brief Gets the capacity of sessions that can be stored.
 *
 * Calculates and returns the total number of sessions that can be stored based on the port range.
 *
 * @return int The total capacity of sessions.
 */
int SessionStorage::getSessionCapacity() const {
    return this->maxPort-this->minPort;
}

void SessionStorage::freePort(int i) {
    logger->info("| SessionStorage.freePort | Freeing port {}", i);
    for(int j = 0; j < this->sessionList.size(); j++){
        if(this->sessionList[j].serverPort == i){
            this->sessionList.erase(this->sessionList.begin()+j);
            return;
        }
    }
    throw std::runtime_error("Session not found !");
}


int SessionStorage::getFreePort() {
    auto usedPorts = std::vector<int>();
    for(auto & i : this->sessionList){
        usedPorts.push_back(i.serverPort);
    }
    // intersection between usedPorts and [minPort, maxPort]
    auto freePorts = std::vector<int>();
    for(int i = this->minPort; i <= this->maxPort; i++){
        if(std::find(usedPorts.begin(), usedPorts.end(), i) == usedPorts.end()){
            freePorts.push_back(i);
        }
    }
    if(freePorts.empty()){
        throw std::runtime_error("Out of port !");
    }
    return freePorts[0];


}
