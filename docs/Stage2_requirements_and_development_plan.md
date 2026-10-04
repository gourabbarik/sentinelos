# Stage 2 — Requirements and Development Plan

## Requirements

SentinelOS needs to run on Linux and use C/C++ as the main implementation languages.

The system needs to:

- Process a sequence of jobs using a worker process.
- Keep a persistent journal of job processing.
- Detect when the worker fails.
- Identify jobs that were left unfinished.
- Restart the worker.
- Attempt to recover unfinished jobs.
- Limit the number of recovery attempts.
- Monitor the worker using Linux `/proc`.
- Provide basic monitoring through TCP.
- Provide a Linux character device for diagnostic events.

The project must remain software-only and should use Linux and C/C++ features that are relevant to the problem.

The main recovery system should not depend on the character device being available.

## Development Plan

The project was developed in small stages so that each major component could be built and tested before moving to the next one.

The main development order was:

1. Create the basic Job model.
2. Add the Job Queue and Job Manager.
3. Implement the Journal.
4. Build the Worker process.
5. Add the Supervisor and Linux process management.
6. Add failure detection and job recovery.
7. Add retry handling for failed recovery attempts.
8. Add `/proc` process monitoring.
9. Add TCP monitoring.
10. Add the Linux character-device diagnostic channel.
11. Build and test the complete system.

The most important part of the development was the recovery path. Normal job processing was completed first, followed by controlled worker failure and recovery testing.

Each major component was tested before it was considered part of the working system.
