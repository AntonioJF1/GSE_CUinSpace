#include "sensors.h"

// -------------------------------
// initSensors() Function
// -------------------------------
// Initializing all sensors (pressure transducers, load cell)
void initSensors(HX711 &loadCell, long &loadCellOffset, float PT_MinV[])
{
  // Initializing load cell communication through HX711
  loadCell.begin(loadCell_DT, loadCell_CLK);

  // Taring load cell
  loadCellOffset = tareLoadCell(loadCell);

  // Calibrating pressure transducers
  for (uint8_t i=0; i<NUM_PT; i++)
  {
    PT_MinV[i] = calibratePressure(pressurePins[i]);
  }
}


// -------------------------------
// readSensors() Function
// -------------------------------
// Reading all sensors and placing their readings into the telemetry packet
// Reminder: All values are 100x to retain 2 decimal places
void readSensors(HX711 &loadCell, long &loadCellOffset, float PT_MinV[], TelemetryPacket &t)
{
  // Reading raw load cell bin output
  long raw = loadCell.read_average(3);
  // Offseting raw output
  long corrected = raw - loadCellOffset;
  // Calculating load cell lb measurement
  float force_lb = corrected / loadCellCalibrationFactor;
  // Storing load cell lb measurement in telemetry packet
  // Note: Value is stored *100 of measured value and type cast into int
  t.loadCell = (uint32_t)(force_lb * 100);

  // Reading pressure transducers
  for (uint8_t i=0; i<NUM_PT; i++)
  {
    // Reading voltage output of pin
    float voltage = (analogRead(pressurePins[i]) / ADC_Resolution) * PT_RefV;

    // Converting voltage to pressure
    float pressure = (voltage - PT_MinV[i]) * (PT_MaxP[i]) / (PT_MaxV - PT_MinV[i]);

    // Collecting edge case
    if (pressure < 0) pressure = 0;

    t.pressure[i] = pressure;
  }
}

// -------------------------------
// calibratePressure() Function
// -------------------------------
// Calibrating pressure sensors (returning voltage where psi = 0);
float calibratePressure(uint8_t Pin_Num)
{
  // Averaging out many pressure readings (bin)
  long sum = 0;
  for (uint8_t i=0; i<50; i++)
  {
    sum += analogRead(Pin_Num);
  } 
  float avgADC = sum / 50.0;

  // Calculating voltage where pressure is 0 psi (gauge)
  float zeroV = (avgADC / ADC_Resolution) * PT_RefV;

  // Returnign 0 psi voltage measurment
  return zeroV;
}


// -------------------------------
// tareLoadCell() Function
// -------------------------------
// Taring the load cell 
long tareLoadCell(HX711 &loadCell)
{
  long offset = loadCell.read_average(20);
  return offset;
}