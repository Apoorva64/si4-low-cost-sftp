//
// Created by tombe on 29/12/2023.
//

#include "SessionStorage.h"

/**
 * @brief Constructs a SessionStorage object with a range of ports.
 *
 * Initializes the SessionStorage with a minimum and maximum port range, sets the current free port to the minimum port,
 * and allocates a list to store session information based on the range of ports provided.
 *
 * @param minPort The minimum port number available for sessions.
 * @param maxPort The maximum port number available for sessions.
 */
SessionStorage::SessionStorage(int minPort, int maxPort) : minPort(minPort), maxPort(maxPort), currentFreePort(minPort), sessionList(maxPort-minPort) {
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
    if(this->currentFreePort > this->maxPort){
        throw std::runtime_error("Out of port !");
    }

    SESSION_CLIENT new_session;

    new_session.clientPort = clientPort;
    new_session.serverPort = this->currentFreePort;

    int index = this->currentFreePort - this->minPort;

    this->sessionList[index] = new_session;

    this->currentFreePort++;

    return index;
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
    return this->sessionList[id];
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
