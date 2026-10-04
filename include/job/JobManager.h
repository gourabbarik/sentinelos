#ifndef SENTINELOS_JOB_MANAGER_H
#define SENTINELOS_JOB_MANAGER_H

#include "job/Job.h"
#include "job/JobQueue.h"

class JobManager {
private:
    JobQueue queue;

public:
    JobManager();

    bool hasNextJob() const;
    Job getNextJob();

    bool findJob(
        const std::string& jobId,
        Job& job) const;
};

#endif
