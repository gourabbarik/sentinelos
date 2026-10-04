#ifndef SENTINELOS_JOB_H
#define SENTINELOS_JOB_H

#include <string>

class Job {
private:
    std::string id;
    std::string operation;
    std::string item;
    int value;

public:
    Job(const std::string& id,
        const std::string& operation,
        const std::string& item,
        int value);

    std::string getId() const;
    std::string getOperation() const;
    std::string getItem() const;
    int getValue() const;
};

#endif
