#ifndef SENTINELOS_RECOVERY_MANAGER_H
#define SENTINELOS_RECOVERY_MANAGER_H

#include <string>
#include <vector>

class RecoveryManager {
public:
    std::vector<std::string> findUnfinishedJobs(
        const std::string& journalFile);
};

#endif
