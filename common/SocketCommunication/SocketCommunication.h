//
// Created by appad on 23/12/2023.
//

#ifndef SECTRANS_SOCKETCOMMUNICATION_H
#define SECTRANS_SOCKETCOMMUNICATION_H

class SocketCommunication {
public:
    SocketCommunication(int inPort, int outPort);
    char *readBuffer;
    char *writeBuffer;
    int inPort;
    int outPort;
    void start() const;

    void send(const std::string& msg) const;

    [[nodiscard]] std::string receiveString() const;

    virtual void handleMessage(const std::string& msg) const;

    [[noreturn]] void run() const;

    void test() const;
};



#endif //SECTRANS_SOCKETCOMMUNICATION_H
