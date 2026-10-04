# SentinelOS

## Linux Background Job Monitoring and Recovery System

SentinelOS is a Linux-based system for monitoring a background worker that processes a sequence of jobs.

The main problem is worker failure during job processing. If the worker stops in the middle of a job, SentinelOS uses the job journal to determine which work may have been left unfinished and attempts to recover it after restarting the worker.

The basic flow of the project is:

**WORK → OBSERVE → DETECT → RECOVER → VERIFY**

The project is implemented using C/C++ and Linux system-programming concepts. The worker runs as a separate Linux process and is managed by a supervisor. A persistent journal records the state of jobs so that unfinished work can be identified after a failure.

The project also includes `/proc`-based process monitoring and a simple TCP monitoring system. A Linux character device is used as an additional channel for diagnostic events.

The main components are:

- Job
- Job Queue
- Job Manager
- Worker
- Journal
- Supervisor
- Recovery Manager
- `/proc` Process Monitor
- TCP Monitoring Server
- TCP Monitoring Client
- Linux Character Device

The project is software-only and is designed to demonstrate Linux process management, job recovery, system programming, and basic monitoring without using databases, web applications, containers, or other external infrastructure.

The main goal is to build a system that can be understood, tested, and explained clearly rather than adding unnecessary complexity.
