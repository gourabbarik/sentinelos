#ifndef SENTINELOS_WORKER_H
#define SENTINELOS_WORKER_H

#include "job/Job.h"
#include "journal/Journal.h"

class Worker {
public:
    void execute(const Job& job,
                 Journal& journal,
                 bool simulateCrash = false);
};

#endif
