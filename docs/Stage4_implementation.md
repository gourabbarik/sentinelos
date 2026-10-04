# Stage 4 — Implementation

## Implementation

SentinelOS was implemented as a set of small C++ components instead of putting the whole system into one large program.

The main implementation is divided between `include/`, `src/`, and `driver/`.

## Job Processing

The `Job` class represents a single job with its basic information.

`JobQueue` provides a FIFO queue for pending jobs, while `JobManager` manages the jobs used by the worker.

The current example workload contains jobs such as:

```text
JOB001 CREATE ITEM_A 10
JOB002 CREATE ITEM_B 20
JOB003 CREATE ITEM_C 30
```

## Journal

The `Journal` component records job state in `sentinel_journal.log`.

A job is first recorded as `PROCESSING`. After successful processing, a `COMPLETED` entry is written.

This makes it possible to identify a job that was being processed when the Worker stopped.

## Worker

The Worker processes the jobs and updates the journal.

A controlled failure option was added for testing:

```bash
./worker --crash-after 2
```

With this option, the Worker completes the first two jobs and then fails while processing the next job.

The Worker also has recovery support for processing an unfinished job again.

## Supervisor

The `Supervisor` manages the Worker as a separate Linux process.

It uses:

- `fork()`
- `exec()`
- `waitpid()`

The Supervisor starts the Worker, monitors its execution, and checks its exit status.

After a failure, it starts a recovery Worker.

The Supervisor also uses `ProcMonitor` to read information about the Worker from `/proc`.

## Recovery

`RecoveryManager` checks the journal and identifies jobs that have a `PROCESSING` entry without a corresponding completed state.

The Supervisor then attempts to recover the unfinished job.

Recovery is limited to three attempts.

The recovery path was tested in both successful and failed-recovery cases.

## Process Monitoring

`ProcMonitor` reads Linux `/proc` information for the Worker process.

The system can report information such as:

- PID
- process state
- memory usage
- thread count
- CPU time

This information is also used by the TCP monitoring server.

## TCP Monitoring

A simple TCP server and client were implemented using Linux socket functions.

The server listens on:

```text
127.0.0.1:5000
```

The client supports:

```text
STATUS
JOBS
LOG
```

`STATUS` provides information about the monitored Worker, while `JOBS` and `LOG` provide journal information.

The implementation uses a simple text-based protocol. No HTTP or REST layer is used.

## Character Device

The `driver/` directory contains the Linux character-device module.

The device is:

```text
/dev/sentinel_events
```

The driver implements the basic character-device operations needed by the project, including `open`, `read`, and `write`.

The C++ `EventReporter` component writes diagnostic events to the device when it is available.

The character device is optional. The main SentinelOS recovery flow does not depend on the driver being loaded.

## Build

The main project is built using the root `Makefile`.

The main executables are:

```text
worker
sentinelos
monitor_server
monitor_client
```

.
