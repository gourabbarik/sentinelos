#include "recovery/RecoveryManager.h"

#include <fstream>
#include <set>
#include <stdexcept>

std::vector<std::string> RecoveryManager::findUnfinishedJobs(
    const std::string& journalFile) {

    std::ifstream file(journalFile);

    if (!file) {
        throw std::runtime_error("Unable to open journal file");
    }

    std::set<std::string> processingJobs;
    std::set<std::string> completedJobs;

    std::string jobId;
    std::string state;

    while (file >> jobId >> state) {
        if (state == "PROCESSING") {
            processingJobs.insert(jobId);
        }
        else if (state == "COMPLETED") {
            completedJobs.insert(jobId);
        }
    }

    std::vector<std::string> unfinishedJobs;

    for (const std::string& id : processingJobs) {
        if (completedJobs.find(id) == completedJobs.end()) {
            unfinishedJobs.push_back(id);
        }
    }

    return unfinishedJobs;
}
