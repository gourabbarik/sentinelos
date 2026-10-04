#include "job/Job.h"

Job::Job(const std::string& id,
         const std::string& operation,
         const std::string& item,
         int value)
    : id(id), operation(operation), item(item), value(value) {
}

std::string Job::getId() const {
    return id;
}

std::string Job::getOperation() const {
    return operation;
}

std::string Job::getItem() const {
    return item;
}

int Job::getValue() const {
    return value;
}
