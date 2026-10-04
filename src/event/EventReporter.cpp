#include "event/EventReporter.h"

#include <fcntl.h>
#include <unistd.h>

void EventReporter::report(const std::string& event) {
    int fd = open("/dev/sentinel_events", O_WRONLY);

    if (fd < 0) {
        return;
    }

    write(fd, event.c_str(), event.size());

    close(fd);
}
