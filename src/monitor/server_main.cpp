#include "monitor/MonitorServer.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr
            << "Usage: ./monitor_server <worker-pid>\n";

        return 1;
    }

    int workerPid =
        std::stoi(argv[1]);

    const int MONITOR_PORT = 5000;

    std::cout
        << "MonitorServer: monitoring worker PID "
        << workerPid
        << '\n';

    MonitorServer server(workerPid);

    if (!server.start(MONITOR_PORT)) {
        return 1;
    }

    return 0;
}
