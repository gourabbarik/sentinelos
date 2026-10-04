#ifndef SENTINELOS_JOB_QUEUE_H
#define SENTINELOS_JOB_QUEUE_H

#include <queue>
#include "job/Job.h"

class JobQueue {
private:
    std::queue<Job> jobs;

public:
    void addJob(const Job& job);
    bool isEmpty() const;
    int size() const;
    Job getNextJob();
};

#endif
