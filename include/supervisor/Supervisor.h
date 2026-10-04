#ifndef SENTINELOS_SUPERVISOR_H
#define SENTINELOS_SUPERVISOR_H

#include <string>
#include "monitor/ProcMonitor.h"

class Supervisor {
public:
    int startWorker();

    int recoverWorker(
        const std::string& jobId,
        bool simulateCrash = false
    );

private:
    void monitorWorker(int pid);
};

#endif
