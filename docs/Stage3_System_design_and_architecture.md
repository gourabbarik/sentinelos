### Worker

The Worker is responsible for processing jobs.

It runs as a separate Linux process. Before processing a job, it records a `PROCESSING` entry in the journal. After successful processing, it records `COMPLETED`.

The Worker also supports controlled failure so that the recovery mechanism can be tested.

### Journal

The Journal stores job state in a file.

For example:

```text
JOB001 PROCESSING
JOB001 COMPLETED
JOB002 PROCESSING
JOB002 COMPLETED
```

If a job has a `PROCESSING` entry without a later `COMPLETED` entry, it can be identified as unfinished.

### Supervisor

The Supervisor manages the Worker process.

It uses Linux process-management functions such as:

- `fork()`
- `exec()`
- `waitpid()`

It starts the Worker and waits for its termination. When the Worker fails, the Supervisor starts the recovery process.

The Supervisor also uses the `/proc` monitor to collect information about the Worker.

### Recovery Manager

`RecoveryManager` checks the journal for unfinished jobs.

When an unfinished job is found, the system starts a recovery Worker and attempts to process the job again.

Recovery is limited to a fixed number of attempts.

### Process Monitor

`ProcMonitor` reads information from Linux `/proc`.

It is used to obtain information about the Worker such as:

- PID
- process state
- memory usage
- thread count
- CPU time

This gives the Supervisor and TCP monitoring server a way to inspect the Worker process.

### TCP Monitoring

The project contains a simple TCP server and client.

The server provides three basic commands:

```text
STATUS
JOBS
LOG
```

`STATUS` shows Worker information, while `JOBS` and `LOG` provide information from the job journal.

The TCP part is intentionally simple and uses normal Linux socket programming.

### Character Device

The project also contains a Linux character device named:

```text
/dev/sentinel_events
```

The C++ application can send diagnostic events to this device.

The character device is optional for the main system. Job processing and recovery continue to work even when the device is not available.

## Recovery Flow

The important part of the architecture is the recovery path.

```text
Worker processes job
        ↓
Journal records PROCESSING
        ↓
Worker fails
        ↓
Supervisor detects termination
        ↓
Recovery Manager checks journal
        ↓
Unfinished job is found
        ↓
Recovery Worker starts
        ↓
Job is processed again
        ↓
Journal records COMPLETED
```

This recovery flow is the main part of SentinelOS.

## Source Code Organization

The project keeps header files and implementation files separate.

```text
include/
    Component headers

src/
    Component implementations

driver/
    Linux character-device code
```

The `include` directory contains the C++ interfaces, while `src` contains their implementations.

The `driver` directory contains the Linux kernel module and its related files.
