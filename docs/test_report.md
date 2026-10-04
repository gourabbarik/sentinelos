# SentinelOS Test Report

## Test Summary

The project was tested from individual components up to the complete recovery flow.

The main focus was verifying that SentinelOS can detect a Worker failure, identify unfinished work, restart the Worker, and recover the unfinished job.

## 1. Build Test

**Command:**

```bash
make clean
make
```

**Expected:**
All project executables should compile successfully without compiler warnings.

**Result:**
The build completed successfully with `-Wall -Wextra` and no compiler warnings.

**Status:** PASS

**Executables built:**

- `worker`
- `sentinelos`
- `monitor_server`
- `monitor_client`

## 2. Normal Worker Test

**Command:**

```bash
./worker --crash-after 2
```

This test was used to verify the Worker and controlled failure mechanism.

**Observed result:**

```text
Worker: processing JOB001: CREATE ITEM_A 10
Worker: JOB001 completed
Worker: processing JOB002: CREATE ITEM_B 20
Worker: JOB002 completed
Worker: processing JOB003: CREATE ITEM_C 30
Worker: simulated crash during JOB003
```

The Worker completed the first two jobs and failed while processing `JOB003`.

**Status:** PASS

## 3. Complete Recovery Test

**Command:**

```bash
./sentinelos
```

The Worker was intentionally allowed to fail during `JOB003`.

The system then:

1. Detected the Worker failure.
2. Checked the journal.
3. Identified `JOB003` as unfinished.
4. Started a recovery Worker.
5. Processed `JOB003` again.
6. Recorded the job as completed.

The Supervisor also reported `/proc` information for the original and recovery Worker.

**Status:** PASS

## 4. Recovery Retry Test

**Command:**

```bash
./sentinelos --recovery-crash
```

The recovery Worker was intentionally made to fail.

**Observed result:**

- Recovery attempt 1 failed.
- Recovery attempt 2 failed.
- Recovery attempt 3 failed.
- The recovery limit was reached.
- The application returned a failure status.

The configured retry limit is:

```text
MAX_RETRIES = 3
```

**Status:** PASS

## 5. `/proc` Monitoring Test

The Supervisor was tested with `/proc` monitoring enabled.

The Worker information reported by the system included:

- PID
- Process state
- RSS memory
- Thread count
- CPU time

**Example:**

```text
Supervisor: worker monitor - PID=304055 STATE=R RSS=1788 KB THREADS=1 CPU_TIME=0
```

The recovery Worker was also monitored after it was started.

**Status:** PASS

## 6. TCP Monitoring Test

The monitoring server was started on:

```text
127.0.0.1:5000
```

The following commands were tested:

```text
STATUS
JOBS
LOG
```

### STATUS

A test was performed using a Worker PID that was no longer available.

The server correctly returned:

```text
Worker PID: 301152
State: unavailable
```

### JOBS

The command returned the recorded journal information.

### LOG

The command also returned the journal information successfully.

**Status:** PASS

## 7. Character Device Test

The Linux character-device module was built and loaded successfully.

The device was available as:

```text
/dev/sentinel_events
```

A diagnostic event was written to the device and read back successfully.

The module was then unloaded successfully.

**Status:** PASS

## 8. Driver-Independent Recovery Test

The character device was removed from the running system and SentinelOS was executed again.

The core job recovery flow still worked without the character device.

This verified that the character device is an additional diagnostic component and is not required for the main recovery mechanism.

**Status:** PASS

## 9. Sanitizer Test

The main C++ components were also tested with AddressSanitizer and UndefinedBehaviorSanitizer builds.

Normal execution and failure/recovery scenarios were exercised.

No sanitizer errors were observed during these tests.

**Status:** PASS

## Final Test Result

The important SentinelOS behavior was verified successfully:

```text
Worker processes jobs
        ↓
Worker failure occurs
        ↓
Supervisor detects failure
        ↓
Journal is checked
        ↓
Unfinished job is identified
        ↓
Recovery Worker starts
        ↓
Unfinished job is processed
        ↓
Job is completed
```

The project also successfully demonstrated `/proc` monitoring, TCP monitoring, character-device operation, recovery retry handling, and recovery without the character device.
