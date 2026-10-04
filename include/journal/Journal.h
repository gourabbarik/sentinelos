#ifndef SENTINELOS_JOURNAL_H
#define SENTINELOS_JOURNAL_H

#include "job/Job.h"
#include <string>

class Journal {
private:
    std::string filename;

public:
    Journal(const std::string& filename);

    void markProcessing(const Job& job);
    void markCompleted(const Job& job);
};

#endif
