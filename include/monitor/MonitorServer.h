#ifndef SENTINELOS_MONITOR_SERVER_H
#define SENTINELOS_MONITOR_SERVER_H

#include "monitor/ProcMonitor.h"

#include <string>

class MonitorServer {
private:
    int serverSocket;
    int workerPid;

public:
    explicit MonitorServer(int workerPid);

    bool start(int port);

private:
    void handleClient(int clientSocket);

    std::string buildStatus();
    std::string buildJobs();
    std::string buildLog();

    void sendResponse(
        int clientSocket,
        const std::string& response);
};

#endif
