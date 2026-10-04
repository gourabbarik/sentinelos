#ifndef SENTINELOS_MONITOR_CLIENT_H
#define SENTINELOS_MONITOR_CLIENT_H

#include <string>

class MonitorClient {
private:
    int clientSocket;

public:
    MonitorClient();

    bool connectToServer(
        const std::string& address,
        int port);

    bool sendCommand(
        const std::string& command);
};

#endif
