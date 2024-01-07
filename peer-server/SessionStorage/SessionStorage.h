//
// Created by tombe on 29/12/2023.
//

#ifndef SECTRANS_SESSIONSTORAGE_H
#define SECTRANS_SESSIONSTORAGE_H

#include "OpenSSL.h"
#include "spdlog/logger.h"

/**
 * @struct session_st
 * @brief This structure represents a session between a client and a server.
 *
 * The session_st structure contains the port numbers for the client and the server.
 */
struct session_st{
    int clientPort; ///< The port number of the client.
    int serverPort; ///< The port number of the server.
};

typedef struct session_st SESSION_CLIENT; ///< Defines SESSION_CLIENT as an alias for struct session_st.

/**
 * @class SessionStorage
 * @brief This class provides functionalities for managing sessions.
 *
 * The SessionStorage class provides functionalities for generating a new session, retrieving a session by its ID,
 * and getting the capacity of sessions that can be stored.
 */
class SessionStorage {
public:
    /**
     * @brief Construct a new SessionStorage object with a range of ports.
     *
     * @param minPort The minimum port number available for sessions.
     * @param maxPort The maximum port number available for sessions.
     */
    SessionStorage(int minPort, int maxPort);

    /**
     * @brief Generate a new session for a client.
     *
     * @param clientPort The port number of the client.
     * @return The session ID of the newly created session.
     */
    int generateSession(int clientPort);

    /**
     * @brief Retrieve a session by its ID.
     *
     * @param id The session ID.
     * @return The session information corresponding to the session ID.
     */
    SESSION_CLIENT getSession(int id);

    /**
     * @brief Get the capacity of sessions that can be stored.
     *
     * @return The total capacity of sessions.
     */
    [[nodiscard]] int getSessionCapacity() const;

    void freePort(int i);

private:
    std::vector<SESSION_CLIENT> sessionList; ///< The list of sessions.
    int minPort; ///< The minimum port number available for sessions.
    int maxPort; ///< The maximum port number available for sessions.
    int currentFreePort; ///< The current free port number.
    int getFreePort();


    std::shared_ptr<spdlog::logger> logger;
};

extern SessionStorage *sessionStorage; ///< The global SessionStorage object.

#endif //SECTRANS_SESSIONSTORAGE_H
