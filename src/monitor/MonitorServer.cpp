#include "monitor/MonitorServer.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

MonitorServer::MonitorServer(int pid)
    : serverSocket(-1),
      workerPid(pid) {
}

bool MonitorServer::start(int port) {
    serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (serverSocket < 0) {
        std::cerr
            << "MonitorServer: socket failed\n";
        return false;
    }

    int reuse = 1;

    setsockopt(
        serverSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &reuse,
        sizeof(reuse)
    );

    sockaddr_in serverAddress{};

    serverAddress.sin_family =
        AF_INET;

    serverAddress.sin_addr.s_addr =
        htonl(INADDR_LOOPBACK);

    serverAddress.sin_port =
        htons(port);

    if (bind(
            serverSocket,
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof(serverAddress)) < 0) {

        std::cerr
            << "MonitorServer: bind failed\n";

        close(serverSocket);
        serverSocket = -1;

        return false;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr
            << "MonitorServer: listen failed\n";

        close(serverSocket);
        serverSocket = -1;

        return false;
    }

    std::cout
        << "MonitorServer: listening on port "
        << port
        << '\n';

    while (true) {
        int clientSocket =
            accept(
                serverSocket,
                nullptr,
                nullptr
            );

        if (clientSocket < 0) {
            break;
        }

        handleClient(clientSocket);

        close(clientSocket);
    }

    close(serverSocket);
    serverSocket = -1;

    return true;
}

void MonitorServer::handleClient(int clientSocket) {
    char buffer[1024]{};

    ssize_t bytesRead =
        read(
            clientSocket,
            buffer,
            sizeof(buffer) - 1
        );

    if (bytesRead <= 0) {
        return;
    }

    buffer[bytesRead] = '\0';

    std::string command(buffer);

    while (!command.empty() &&
           (command.back() == '\n' ||
            command.back() == '\r')) {

        command.pop_back();
    }

    if (command == "STATUS") {
        sendResponse(
            clientSocket,
            buildStatus()
        );
    }
    else if (command == "JOBS") {
        sendResponse(
            clientSocket,
            buildJobs()
        );
    }
    else if (command == "LOG") {
        sendResponse(
            clientSocket,
            buildLog()
        );
    }
    else {
        sendResponse(
            clientSocket,
            "ERROR: unknown command\n"
        );
    }
}

std::string MonitorServer::buildStatus() {
    ProcMonitor monitor;
    ProcessInfo info;

    std::ostringstream output;

    if (!monitor.readProcessInfo(
            workerPid,
            info)) {

        output
            << "Worker PID: "
            << workerPid
            << '\n'
            << "State: unavailable\n";

        return output.str();
    }

    output
        << "Worker PID: "
        << info.pid
        << '\n'
        << "State: "
        << info.state
        << '\n'
        << "Memory: "
        << info.memoryKb
        << " KB\n"
        << "Threads: "
        << info.threads
        << '\n'
        << "CPU_TIME: "
        << info.cpuTime
        << '\n';

    return output.str();
}

std::string MonitorServer::buildJobs() {
    std::ifstream file(
        "sentinel_journal.log"
    );

    if (!file.is_open()) {
        return "Unable to open journal\n";
    }

    std::ostringstream output;
    std::string line;

    while (std::getline(file, line)) {
        output << line << '\n';
    }

    return output.str();
}

std::string MonitorServer::buildLog() {
    return buildJobs();
}

void MonitorServer::sendResponse(
    int clientSocket,
    const std::string& response) {

    write(
        clientSocket,
        response.c_str(),
        response.size()
    );
}
