#include "monitor/MonitorClient.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

MonitorClient::MonitorClient()
    : clientSocket(-1) {
}

bool MonitorClient::connectToServer(
    const std::string& address,
    int port) {

    clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (clientSocket < 0) {
        std::cerr
            << "MonitorClient: socket failed\n";

        return false;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(
            AF_INET,
            address.c_str(),
            &serverAddress.sin_addr) <= 0) {

        std::cerr
            << "MonitorClient: invalid address\n";

        close(clientSocket);
        clientSocket = -1;

        return false;
    }

    if (connect(
            clientSocket,
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof(serverAddress)) < 0) {

        std::cerr
            << "MonitorClient: connection failed\n";

        close(clientSocket);
        clientSocket = -1;

        return false;
    }

    return true;
}

bool MonitorClient::sendCommand(
    const std::string& command) {

    if (clientSocket < 0) {
        return false;
    }

    std::string request =
        command + "\n";

    if (write(
            clientSocket,
            request.c_str(),
            request.size()) < 0) {

        return false;
    }

    char buffer[4096]{};

    ssize_t bytesRead = read(
        clientSocket,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytesRead <= 0) {
        return false;
    }

    buffer[bytesRead] = '\0';

    std::cout << buffer;

    close(clientSocket);
    clientSocket = -1;

    return true;
}
