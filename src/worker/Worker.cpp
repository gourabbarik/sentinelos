#include "worker/Worker.h"

#include <cstdlib>
#include <iostream>

void Worker::execute(const Job& job,
                     Journal& journal,
                     bool simulateCrash) {

    journal.markProcessing(job);

    std::cout << "Worker: processing "
              << job.getId() << ": "
              << job.getOperation() << " "
              << job.getItem() << " "
              << job.getValue() << '\n';

    if (simulateCrash) {
        std::cerr << "Worker: simulated crash during "
                  << job.getId() << '\n';

        std::exit(1);
    }

    std::cout << "Worker: "
              << job.getId()
              << " completed\n";

    journal.markCompleted(job);
}
