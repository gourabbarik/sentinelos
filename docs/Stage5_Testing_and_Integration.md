# Stage 5 — Testing and Integration

## Testing

The project was tested component by component and then as a complete system.

The main tests focused on the worker failure and recovery flow because this is the main purpose of SentinelOS.

## Normal Job Processing

The normal SentinelOS execution was tested with the default job set.

The Worker successfully processed the jobs and recorded their states in the journal.

The build was also tested using:

```bash
make clean
make
```

The main executables compiled successfully with `-Wall -Wextra` and without compiler warnings.

## Worker Failure Test

Controlled Worker failure was tested using:

```bash
./worker --crash-after 2
```

The Worker completed the first two jobs and then simulated a failure while processing `JOB003`.

This confirmed that the failure-injection mechanism works.

## Job Recovery Test

The complete recovery flow was tested using:

```bash
./sentinelos
```

During the test, the Worker failed while processing `JOB003`.

The system then:

1. Detected the Worker failure.
2. Checked the journal.
3. Identified `JOB003` as unfinished.
4. Started a recovery Worker.
5. Processed `JOB003` again.
6. Recorded the job as completed.

The recovery test completed successfully.

## Recovery Retry Test

The recovery limit was tested using:

```bash
./sentinelos --recovery-crash
```

The recovery Worker was intentionally made to fail.

The system attempted recovery three times and then stopped after reaching the retry limit.

The program returned a failure status after the recovery limit was reached.

## `/proc` Monitoring Test

The Supervisor was tested after adding `/proc` monitoring.

During execution, it reported information such as:

```text
PID
STATE
RSS
THREADS
CPU_TIME
```

This confirmed that the Supervisor was able to inspect the Worker through Linux `/proc`.

## TCP Monitoring Test

The TCP monitoring server was tested on:

```text
127.0.0.1:5000
```

The following commands were tested through the monitoring client:

```text
STATUS
JOBS
LOG
```

`STATUS` correctly handled a Worker PID that was no longer available and reported:

```text
State: unavailable
```

`JOBS` and `LOG` returned the journal information successfully.

## Character Device Test

The Linux character device was built and loaded as a kernel module.

The device:

```text
/dev/sentinel_events
```

was tested for writing and reading diagnostic events.

The module was also unloaded successfully.

SentinelOS was then run without the character device, and the main job recovery flow still worked. This confirmed that the character device is not required for core recovery.
