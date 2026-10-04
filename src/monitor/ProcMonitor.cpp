#include "monitor/ProcMonitor.h"

#include <fstream>
#include <sstream>

bool ProcMonitor::readProcessInfo(
    int pid,
    ProcessInfo& info) {

  	info.pid = pid;
	info.state = '?';
	info.memoryKb = 0;
	info.threads = 0;
	info.cpuTime = 0;
    if (!readStatFile(pid, info)) {
        return false;
    }

    if (!readStatusFile(pid, info)) {
        return false;
    }

    return true;
}

bool ProcMonitor::readStatFile(
    int pid,
    ProcessInfo& info) {

    std::string path =
        "/proc/" + std::to_string(pid) + "/stat";

    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::getline(file, line);

    if (line.empty()) {
        return false;
    }

    /*
     * /proc/<pid>/stat contains:
     *
     * pid (comm) state ppid ...
     *
     * The process name is enclosed in parentheses,
     * so find the final ')' before reading the fields.
     */
    std::size_t closingParen = line.rfind(')');

    if (closingParen == std::string::npos ||
        closingParen + 2 >= line.size()) {
        return false;
    }

    info.state = line[closingParen + 2];

    /*
     * Fields after the process name begin with:
     *
     * state = field 3
     * ppid  = field 4
     * ...
     * utime = field 14
     * stime = field 15
     *
     * We only need CPU time here.
     */

    std::istringstream stream(
        line.substr(closingParen + 2)
    );

    char state;
    long value;

    stream >> state;

    /*
     * Skip fields 4 through 13.
     */
    for (int field = 4; field <= 13; ++field) {
        stream >> value;
    }

    long utime = 0;
    long stime = 0;

    stream >> utime >> stime;

    info.cpuTime = utime + stime;

    return true;
}

bool ProcMonitor::readStatusFile(
    int pid,
    ProcessInfo& info) {

    std::string path =
        "/proc/" + std::to_string(pid) + "/status";

    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream stream(line);

        std::string key;

        if (!(stream >> key)) {
            continue;
        }

        if (key == "VmRSS:") {
            stream >> info.memoryKb;
        }
        else if (key == "Threads:") {
            stream >> info.threads;
        }
    }

    return true;
}

