#include "job/JobQueue.h"

void JobQueue::addJob(const Job& job) {
    jobs.push(job);
}

bool JobQueue::isEmpty() const {
    return jobs.empty();
}

int JobQueue::size() const {
    return jobs.size();
}

Job JobQueue::getNextJob() {
    Job job = jobs.front();
    jobs.pop();
    return job;
}
