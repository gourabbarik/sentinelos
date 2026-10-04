#include "recovery/RecoveryManager.h"
#include "supervisor/Supervisor.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    const int MAX_RETRIES = 3;

    bool forceRecoveryFailure = false;

    if (argc == 2 &&
        std::string(argv[1]) == "--recovery-crash") {
        forceRecoveryFailure = true;
    }

    std::cout << "SentinelOS: starting\n";

    std::remove("sentinel_journal.log");

    Supervisor supervisor;
    RecoveryManager recoveryManager;

    std::cout << "SentinelOS: starting worker\n";

    int workerExitCode = supervisor.startWorker();

    if (workerExitCode == 0) {
        std::cout << "SentinelOS: worker completed normally\n";
        return 0;
    }

    std::cout << "SentinelOS: worker failure detected\n";

    std::vector<std::string> unfinishedJobs =
        recoveryManager.findUnfinishedJobs(
            "sentinel_journal.log"
        );

    if (unfinishedJobs.empty()) {
        std::cout << "SentinelOS: no unfinished jobs found\n";
        return 1;
    }

    std::cout << "SentinelOS: unfinished jobs detected:\n";

    for (const std::string& jobId : unfinishedJobs) {
        std::cout << "  " << jobId << '\n';
    }

    for (const std::string& jobId : unfinishedJobs) {
        bool recovered = false;

        for (int attempt = 1;
             attempt <= MAX_RETRIES;
             ++attempt) {

            std::cout
                << "SentinelOS: recovery attempt "
                << attempt
                << " of "
                << MAX_RETRIES
                << " for "
                << jobId
                << '\n';

            int recoveryExitCode =
                supervisor.recoverWorker(
                    jobId,
                    forceRecoveryFailure
                );

            if (recoveryExitCode == 0) {
                recovered = true;
                break;
            }

            std::cout
                << "SentinelOS: recovery attempt "
                << attempt
                << " failed\n";
        }

        if (!recovered) {
            std::cerr
                << "SentinelOS: recovery limit reached for "
                << jobId
                << '\n';

            return 1;
        }
    }

    std::vector<std::string> remainingJobs =
        recoveryManager.findUnfinishedJobs(
            "sentinel_journal.log"
        );

    if (remainingJobs.empty()) {
        std::cout
            << "SentinelOS: recovery successful\n";

        return 0;
    }

    std::cout
        << "SentinelOS: unfinished jobs remain\n";

    return 1;
}
