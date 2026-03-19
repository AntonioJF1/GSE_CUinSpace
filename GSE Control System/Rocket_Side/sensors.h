#ifndef SENSORS_H
#define SENSORS_H

#include "packets.h"
#include "config.h"
#include <HX711.h> // Load Cell Amplifier (***Need to Download HX711 by Rob Tillaart***)

// -------------------------------
// Key Sensor Constants
// -------------------------------

// Pressure Transducer Constants
constexpr uint8_t PT_MaxP[NUM_PT] = {1000,1600,1600};
constexpr float PT_MaxV = 4.5; // Maximum reading voltage of pressure transducers
constexpr float PT_RefV = 5.0; // Used to calculating bin/psi
constexpr float ADC_Resolution = 1023.0; // Resolution of analog to digital converter

// Load Cell Constants
constexpr float loadCellCalibrationFactor = 4299.18 / 2.20462; // Experimentally determined: (bin/lb) bin/kg / lb/kg


// -------------------------------
// Function Definitions
// -------------------------------

// Initializing all sensors (pressure transducers, load cell)
void initSensors(HX711 &loadCell, long &loadCellOffset, float PT_MinV[]);

// Reading all sensors and placing their readings into the telemetry packet
// Reminder: All values are 100x to retain 2 decimal places
void readSensors(HX711 &loadCell, long &loadCellOffset, float PT_MinV[], TelemetryPacket &t);

// Calibrating pressure sensors (returning voltage where psi = 0);
float calibratePressure(uint8_t Pin_Num);

// Taring the load cell 
long tareLoadCell(HX711 &loadCell);

#endif