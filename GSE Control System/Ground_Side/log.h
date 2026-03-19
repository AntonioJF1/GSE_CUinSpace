#ifndef LOG_H
#define LOG_H

#include "packets.h"
#include "config.h"

// Initializing data logger
void initLogger();

// Starting new data file on microSD card
void startNewLogFile();

// Logs the telemetry received to the microSD card
void logTelemetry(const TelemetryPacket &t);


#endif