#include "supervisor/Supervisor.h"
#include "event/EventReporter.h"

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void Supervisor::monitorWorker(int pid) {
    ProcMonitor monitor;
    ProcessInfo info;

    if (!monitor.readProcessInfo(pid, info)) {
        return;
    }

    std::cout << "Supervisor: worker monitor - PID="
              << info.pid
              << " STATE=" << info.state
              << " RSS=" << info.memoryKb << " KB"
              << " THREADS=" << info.threads
              << " CPU_TIME=" << info.cpuTime
              << '\n';
}

int Supervisor::startWorker() {
    EventReporter reporter;

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Supervisor: fork() failed\n";
        return -1;
    }

    if (pid == 0) {
        std::cout << "Supervisor: starting worker...\n";

        execl(
            "./worker",
            "./worker",
            "--crash-after",
            "2",
            (char*)nullptr
        );

        std::cerr << "Supervisor: exec() failed\n";
        _exit(1);
    }

    std::cout << "Supervisor: worker started with PID "
              << pid << '\n';

    reporter.report("WORKER_STARTED");

    monitorWorker(pid);

    int status = 0;

    if (waitpid(pid, &status, 0) == -1) {
        std::cerr << "Supervisor: waitpid() failed\n";
        return -1;
    }

    if (WIFEXITED(status)) {
        int exitCode = WEXITSTATUS(status);

        if (exitCode != 0) {
            reporter.report("WORKER_FAILED");
        }

        std::cout << "Supervisor: worker exited with code "
                  << exitCode << '\n';

        return exitCode;
    }

    if (WIFSIGNALED(status)) {
        reporter.report("WORKER_FAILED");

        std::cout << "Supervisor: worker terminated by signal "
                  << WTERMSIG(status) << '\n';

        return 128 + WTERMSIG(status);
    }

    return -1;
}

int Supervisor::recoverWorker(
    const std::string& jobId,
    bool simulateCrash) {

    EventReporter reporter;

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Supervisor: fork() failed\n";
        return -1;
    }

    if (pid == 0) {
        std::cout << "Supervisor: starting recovery worker...\n";

        if (simulateCrash) {
            execl(
                "./worker",
                "./worker",
                "--recover",
                jobId.c_str(),
                "--crash",
                (char*)nullptr
            );
        }
        else {
            execl(
                "./worker",
                "./worker",
                "--recover",
                jobId.c_str(),
                (char*)nullptr
            );
        }

        std::cerr << "Supervisor: recovery exec() failed\n";
        _exit(1);
    }

    std::cout << "Supervisor: recovery worker started with PID "
              << pid << '\n';

    reporter.report("WORKER_RESTARTED");

    monitorWorker(pid);

    int status = 0;

    if (waitpid(pid, &status, 0) == -1) {
        std::cerr << "Supervisor: recovery waitpid() failed\n";
        return -1;
    }

    if (WIFEXITED(status)) {
        int exitCode = WEXITSTATUS(status);

        if (exitCode == 0) {
            reporter.report("JOB_RECOVERED");
        }
        else {
            reporter.report("WORKER_FAILED");
        }

        std::cout << "Supervisor: recovery worker exited with code "
                  << exitCode << '\n';

        return exitCode;
    }

    if (WIFSIGNALED(status)) {
        reporter.report("WORKER_FAILED");

        std::cout << "Supervisor: recovery worker terminated by signal "
                  << WTERMSIG(status) << '\n';

        return 128 + WTERMSIG(status);
    }

    return -1;
}
