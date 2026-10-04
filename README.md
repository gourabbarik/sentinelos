# SentinelOS

### Linux Background Job Monitoring and Recovery System

SentinelOS is a Linux-based C++ system designed to handle a simple but practical problem: **what happens when a background Worker fails while processing a job?**

Instead of losing track of the unfinished work, SentinelOS records job progress in a persistent journal, detects Worker failure, identifies potentially unfinished jobs, restarts the Worker, and attempts to recover the unfinished job.

## Problem

A background Worker may terminate while a job is being processed. After the failure, the system needs a way to determine what work was in progress and continue from there.

SentinelOS addresses this using a persistent job journal and a controlled recovery process.

## How SentinelOS Works

The main flow is:

**WORK → OBSERVE → DETECT → RECOVER → VERIFY**

1. The Worker processes jobs.
2. Job progress is recorded in the journal.
3. The Supervisor manages the Worker process.
4. If the Worker fails, the journal is checked for unfinished jobs.
5. A recovery Worker is started to process the unfinished job.
6. Recovery is retried up to the configured limit.

## Key Features

- Background job processing
- Persistent job journal
- Controlled Worker failure simulation
- Worker failure detection and restart
- Unfinished job recovery
- Recovery retry handling
- Linux `/proc` process monitoring
- TCP-based monitoring
- Linux character-device diagnostic events

## Technologies and Concepts

- C++
- C
- Linux process management
- `fork()`, `exec()`, `waitpid()`
- Linux `/proc`
- File I/O and STL
- TCP sockets
- Linux character devices
- Kernel modules
- Mutex and basic synchronization
- GNU Make

## Build

From the project root:

```bash
make clean
make
```

## Run

Run the complete recovery demonstration:

```bash
./sentinelos
```

Test controlled Worker failure:

```bash
./worker --crash-after 2
```

Test recovery retry handling:

```bash
./sentinelos --recovery-crash
```

## TCP Monitoring

Start the monitoring server with a Worker PID:

```bash
./monitor_server <worker-pid>
```

Then, from another terminal:

```bash
./monitor_client STATUS
./monitor_client JOBS
./monitor_client LOG
```

## Project Structure

```text
sentinelos/
├── driver/
├── include/
├── src/
├── docs/
├── Makefile
└── README.md
```
