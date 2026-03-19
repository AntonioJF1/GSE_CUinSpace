#ifndef LORA_H
#define LORA_H

#include "packets.h"
#include "config.h"
#include "crc.h"
#include <SPI.h>
#include <RH_RF95.h>

// -------------------------------
// Key Radio Variables
// -------------------------------
constexpr float LoRa_Freq = 915.0;

// -------------------------------
// Function Definitions
// -------------------------------

// Initializes LoRa Transceiver
// Makes sure every process runs cleanly
void initLoRa(RH_RF95 &LoRa);

// Receiving telemetry from rocket
bool receiveTelemetry(RH_RF95 &LoRa, TelemetryPacket &t);

// Sending commands to rocket
void sendCommands(RH_RF95 &LoRa, CommandPacket &cmd);

// Debugging function that prints out all received telemetry to serial monitor
void debugReceive(const TelemetryPacket &t);

// Debugging function that prints out all sent commands to serial monitor
void debugSend(const CommandPacket &cmd);

#endif