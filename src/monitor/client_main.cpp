#include "monitor/MonitorClient.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr
            << "Usage: ./monitor_client STATUS|JOBS|LOG\n";

        return 1;
    }

    std::string command = argv[1];

    if (command != "STATUS" &&
        command != "JOBS" &&
        command != "LOG") {

        std::cerr
            << "Unknown command: "
            << command
            << '\n';

        return 1;
    }

    MonitorClient client;

    if (!client.connectToServer(
            "127.0.0.1",
            5000)) {

        return 1;
    }

    if (!client.sendCommand(command)) {
        return 1;
    }

    return 0;
}
