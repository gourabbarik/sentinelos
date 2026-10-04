#include "job/JobManager.h"

JobManager::JobManager() {
    queue.addJob(
        Job("JOB001", "CREATE", "ITEM_A", 10)
    );

    queue.addJob(
        Job("JOB002", "CREATE", "ITEM_B", 20)
    );

    queue.addJob(
        Job("JOB003", "CREATE", "ITEM_C", 30)
    );
}

bool JobManager::hasNextJob() const {
    return !queue.isEmpty();
}

Job JobManager::getNextJob() {
    return queue.getNextJob();
}

bool JobManager::findJob(
    const std::string& jobId,
    Job& job) const {

    if (jobId == "JOB001") {
        job = Job("JOB001", "CREATE", "ITEM_A", 10);
        return true;
    }

    if (jobId == "JOB002") {
        job = Job("JOB002", "CREATE", "ITEM_B", 20);
        return true;
    }

    if (jobId == "JOB003") {
        job = Job("JOB003", "CREATE", "ITEM_C", 30);
        return true;
    }

    return false;
}
