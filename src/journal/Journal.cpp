#include "journal/Journal.h"
#include <fstream>
#include <stdexcept>

Journal::Journal(const std::string& filename)
    : filename(filename) {
}

void Journal::markProcessing(const Job& job) {
    std::ofstream file(filename, std::ios::app);

    if (!file) {
        throw std::runtime_error("Unable to open journal file");
    }

    file << job.getId() << " PROCESSING\n";
}

void Journal::markCompleted(const Job& job) {
    std::ofstream file(filename, std::ios::app);

    if (!file) {
        throw std::runtime_error("Unable to open journal file");
    }

    file << job.getId() << " COMPLETED\n";
}

