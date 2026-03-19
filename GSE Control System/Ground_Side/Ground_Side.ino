// Arduino Mega Code for Ground Side GSE System
// Contributors: Carson Mead
// Date Modified: 3/12/26

// -------------------------------
// Ground Side System Overview:
// -------------------------------
// - 1 Arduino Mega
// - 915 MHz LoRa Transceiver
// - 8 Control Switches
// - 1 Abort Switch
// - 2 Control Buttons
// - 4 MAX7129 7 Segment Displays
// - 2 4 Pin LCD Screens
// - 1 MicroSD Data Logger

// -------------------------------
// Including Needed Header Files
// -------------------------------
#include "packets.h"
#include "config.h"
#include "lora.h"
#include "display.h"
#include "log.h"
#include "switches.h"
#include "crc.h"


// -------------------------------
// Initializing Key Objects and Structures
// -------------------------------

// LoRa transceiver
RH_RF95 LoRa(LoRa_CS, LoRa_INT);
// Array of LCD screens
// LiquidCrystal_I2C LCDs[NUM_LCD] = {}
// Seven Segment Display (4 Daisy Chained)
LedControl SevSD(SevSD_DIN,SevSD_CS,SevSD_CLK,NUM_SevSD);
// Initializing telemetry packet object
TelemetryPacket telemetry;
// Initializing command packet object
CommandPacket command = {0};
// Initializing last packet sent variable
long lastPacketSent = millis();


// -------------------------------
// Main Setup
// -------------------------------
void setup() {

  Serial.begin(9600); // Serial Monitor Computer
  
  // Initializing data logger
  initLogger();

  // LoRa Setup
  initLoRa(LoRa);

  // Initializing all displays
  // initDisplays();

  // Setting Up Switches
  initSwitches();

  // Printing that System is Ready
  Serial.println("Ground System Ready");
}


// -------------------------------
// Main Loop
// -------------------------------
void loop() {

  // Receiving Data from the Rocket Side
  receiveTelemetry(LoRa, telemetry);

  // Updating LCDs and 7SDs
  // updateDisplays(LCDS, SevSD);

  // Logging the Data to the OpenLog
  logTelemetry(telemetry);

  if (millis() - lastPacketSent > 50)
  {
    // Reading Switch Commands
    readSwitches(command);

    // Sending Commands to the Rocket Side
    sendCommands(LoRa, command);
  }
  
}



