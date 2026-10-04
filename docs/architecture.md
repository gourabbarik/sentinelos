# SentinelOS Architecture

SentinelOS is organized into separate components for job processing, worker management, recovery, monitoring, and diagnostics.

## Core Components

- **Job** — Represents one unit of work.
- **JobQueue** — Stores pending jobs in FIFO order.
- **JobManager** — Manages the jobs used by the Worker.
- **Worker** — Processes jobs and records their state in the journal.
- **Journal** — Stores persistent job-processing information.
- **Supervisor** — Starts the Worker, monitors it, and handles Worker termination.
- **RecoveryManager** — Checks the journal and identifies unfinished jobs for recovery.
- **ProcMonitor** — Reads Worker information from Linux `/proc`.
- **MonitorServer** — Provides simple TCP-based monitoring.
- **MonitorClient** — Sends monitoring commands to the TCP server.
- **EventReporter** — Sends diagnostic events to the Linux character device.

## How the Components Work Together

The Job Manager provides jobs to the Job Queue, and the Worker processes them.

The Worker records its progress in the Journal. The Supervisor manages the Worker as a separate Linux process and uses `ProcMonitor` to inspect its process information.

If the Worker terminates unexpectedly, the Supervisor checks the Journal through the Recovery Manager. An unfinished job can then be processed again by a recovery Worker.

The TCP monitoring components provide a simple way to inspect Worker and journal information.

The character device provides an additional diagnostic event channel. It is not required for the core job recovery process.

## Recovery

Recovery is based on the job journal.

A job that has a `PROCESSING` entry without a later `COMPLETED` entry is treated as potentially unfinished.

The system attempts to process the unfinished job again. Recovery is limited to three attempts.

## Project Structure

```text
sentinelos/
├── driver/
├── include/
├── src/
├── docs/
├── Makefile
├── README.md
└── .gitignore
