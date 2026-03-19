#include "log.h"

// -------------------------------
// initLogger() Function
// -------------------------------
// Initializing data logger
void initLogger()
{
  // Opening up serial channel 1 (9600 baud)
  // Note: the serial logger only takes in 9600 baud inputs
  Serial1.begin(9600);
  delay(1000);

  // Opening new file on data logger
  startNewLogFile();

  // Entering csv header
  Serial1.println("Time,PT1,PT2,PT3,LoadCell");
}

// -------------------------------
// startNewLogFile() Function
// -------------------------------
// Starting new data file on microSD card
void startNewLogFile()
{
  // Entering command mode 
  // Byte number provided by manufacturer in config file on microSD
  Serial1.write(26);
  delay(100);

  // Creating new file
  Serial1.println("new");
  delay(100);

  // Exiting command mode
  Serial1.write(26);
  delay(100);
}

// -------------------------------
// logTelemetry() Function
// -------------------------------
// Logs the telemetry received to the microSD card
void logTelemetry(const TelemetryPacket &t)
{
  // Printing in .csv form
  Serial1.print(t.timestamp);
  Serial1.print(",");

  for (uint8_t i=0; i<NUM_PT; i++)
  {
    Serial1.print(t.pressure[i]);
    Serial1.print(",");
  }

  Serial1.print(t.loadCell);
  Serial1.println();
}




