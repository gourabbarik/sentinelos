CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

WORKER_SOURCES = \
    src/worker/main.cpp \
    src/worker/Worker.cpp \
    src/job/Job.cpp \
    src/job/JobQueue.cpp \
    src/job/JobManager.cpp \
    src/journal/Journal.cpp

SENTINELOS_SOURCES = \
    src/app/main.cpp \
    src/job/Job.cpp \
    src/journal/Journal.cpp \
    src/recovery/RecoveryManager.cpp \
    src/supervisor/Supervisor.cpp \
    src/monitor/ProcMonitor.cpp \
    src/event/EventReporter.cpp 

MONITOR_SERVER_SOURCES = \
    src/monitor/server_main.cpp \
    src/monitor/MonitorServer.cpp \
    src/monitor/ProcMonitor.cpp

MONITOR_CLIENT_SOURCES = \
    src/monitor/client_main.cpp \
    src/monitor/MonitorClient.cpp

all: worker sentinelos monitor_server monitor_client

worker: $(WORKER_SOURCES)
	$(CXX) $(CXXFLAGS) $(WORKER_SOURCES) -o worker

sentinelos: $(SENTINELOS_SOURCES)
	$(CXX) $(CXXFLAGS) $(SENTINELOS_SOURCES) -o sentinelos

monitor_server: $(MONITOR_SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $(MONITOR_SERVER_SOURCES) -o monitor_server

monitor_client: $(MONITOR_CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(MONITOR_CLIENT_SOURCES) -o monitor_client

clean:
	rm -f worker sentinelos monitor_server monitor_client
