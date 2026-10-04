#ifndef SENTINELOS_PROC_MONITOR_H
#define SENTINELOS_PROC_MONITOR_H

#include <string>

struct ProcessInfo {
    int pid;
    char state;
    long memoryKb;
    int threads;
    long cpuTime;
};
class ProcMonitor {
public:
    bool readProcessInfo(int pid, ProcessInfo& info);

private:
    bool readStatFile(int pid, ProcessInfo& info);
    bool readStatusFile(int pid, ProcessInfo& info);
};

#endif
