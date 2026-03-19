#ifndef LORA_H
#define LORA_H

#include "packets.h"
#include "config.h"
#include "crc.h"
#include <SPI.h> // Serial Peripheral Interface (SCLK,MOSI,MISO,etc.)
#include <RH_RF95.h> // LoRa Transceiver Library (***Need to Download RadioHead by Mike McCauley***)

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

// Receiving commands from ground station
bool receiveCommands(RH_RF95 &LoRa, CommandPacket &cmd, CommandPacket &lastCmd);

// Sending telemetry to ground station (sensor data and actuator position)
void sendTelemetry(RH_RF95 &LoRa, const TelemetryPacket &t);

// Debugging function that prints out all received commands to serial monitor
void debugReceive(const CommandPacket &cmd);

// Debugging function that prints out all sent telemetry to serial monitor
void debugSend(const TelemetryPacket &t);

#endif