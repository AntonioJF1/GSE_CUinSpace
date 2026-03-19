// Arduino Mega Code for Rocket Side GSE System
// Contributors: Carson Mead
// Date Modified: 3/12/26

// -------------------------------
// Rocket Side System Overview:
// -------------------------------
// - 1 Arduino Mega
// - 915 MHz LoRa Transceiver
// - 2 1600 psi Pressure Transducers
// - 1 1000 psi Pressure Transducer
// - 1 10 kN Load Cell
// - 4 20 kgcm Servo Motor Controlled Valves
// - 1 High Pressure Solenoid Valve


// -------------------------------
// Including Needed Header Files
// -------------------------------
#include "packets.h"
#include "config.h"
#include "sensors.h"
#include "lora.h"
#include "actuators.h"
#include "crc.h"

// -------------------------------
// Initializing Key Objects and Structs
// -------------------------------

// LoRa transceiver
RH_RF95 LoRa(LoRa_CS, LoRa_INT);
// Array of servo objects
Servo servos[NUM_SERVO];
// Load cell object
HX711 loadCell;
// Struct to send telemetry to ground station
TelemetryPacket telemetry;
// Struct to send commands to the rocket
CommandPacket command;

// -------------------------------
// Initializing Key Variables
// -------------------------------

// Used for calibrating sensors
float PT_MinV[NUM_PT] = {0.0,0.0,0.0};
// Used for taring load cell
long loadCellOffset = 0; // Number of bins offset
// Used for attaching and detaching servo motors to arduino pins (reduces twitchiness)
bool servoAttachState[NUM_SERVO] = {false,false,false,false};
uint16_t servoActuationStart[NUM_SERVO] = {0,0,0,0};
// Initializing previous command packet to eliminate duplicate sent commands
CommandPacket lastCommand = {0};
// Initializing failsafe to close servos if no commands are sent
uint16_t lastCommandTime = 0;
uint16_t lastTelemetrySent = 0;


// -------------------------------
// Main Setup
// -------------------------------
void setup() {

  // Opening Serial Monitor (For Debugging)
  Serial.begin(9600); // Baud Rate = 9600 bits/s

  // Initializing the LoRa Transceiver
  initLoRa(LoRa);

  // Initializing Sensors
  initSensors(loadCell, loadCellOffset, PT_MinV);

  // Initializing Actuators
  initActuators(servos);

  // Printing to Console State
  Serial.println("Rocket-Side Arduino Ready");
}


// -------------------------------
// Main Loop
// -------------------------------
void loop() {

  // Acting on Commands
  // If statement is not entered if no commands were received or packets did not pass crc check
  if (receiveCommands(LoRa, command, lastCommand))
  {
    applyActuateCommands(servos, servoAttachState, servoActuationStart, lastCommand, command);

    // Taring load cell if commanded
    if (command.tareLoadCellState != lastCommand.tareLoadCellState && command.tareLoadCellState == true)
    {
      loadCellOffset = tareLoadCell(loadCell);
    }
  }

  // Sending Telemetry
  if (millis - lastTelemetrySent > 20)
  {
    // Reading Sensors
    readSensors(loadCell, loadCellOffset, PT_MinV, telemetry);

    // Reading Actuator States
    readActuatorStates(servos, telemetry);

    // Reading Current Time
    telemetry.timestamp = millis();

    // Send Telemetry Data over LoRa
    sendTelemetry(LoRa,telemetry);
  }

  // Detaching servos if they have finished moving
  updateServos(servos, servoAttachState, servoActuationStart);
}
