
#include "worker/Worker.h"
#include "job/JobManager.h"
#include "journal/Journal.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    int crashAfter = -1;
    std::string recoverJobId;
    bool recoveryCrash = false;

    if (argc == 3 &&
        std::string(argv[1]) == "--crash-after") {

        crashAfter = std::stoi(argv[2]);

        if (crashAfter < 1) {
            std::cerr << "Worker: crash-after must be at least 1\n";
            return 1;
        }
    }
    else if (argc >= 3 &&
             std::string(argv[1]) == "--recover") {

        recoverJobId = argv[2];

        if (argc == 4 &&
            std::string(argv[3]) == "--crash") {
            recoveryCrash = true;
        }
    }

    Journal journal("sentinel_journal.log");
    JobManager jobManager;
    Worker worker;

    /*
     * Recovery mode:
     * locate the unfinished job and execute it again.
     */
    if (!recoverJobId.empty()) {
        Job job(
            "",
            "",
            "",
            0
        );

        if (!jobManager.findJob(recoverJobId, job)) {
            std::cerr << "Worker: job not found: "
                      << recoverJobId << '\n';

            return 1;
        }

        std::cout << "Worker: recovering "
                  << job.getId() << '\n';

        worker.execute(
            job,
            journal,
            recoveryCrash
        );

        return 0;
    }

    /*
     * Normal execution:
     * JobManager supplies jobs from JobQueue.
     */
    int jobsCompleted = 0;

    while (jobManager.hasNextJob()) {
        Job job = jobManager.getNextJob();

        /*
         * --crash-after 2 means:
         * complete two jobs, then crash while
         * processing the third job.
         */
        bool simulateCrash =
            (crashAfter != -1 &&
             jobsCompleted >= crashAfter);

        worker.execute(
            job,
            journal,
            simulateCrash
        );

        ++jobsCompleted;
    }

    return 0;
}
