#ifndef PACKETS_H
#define PACKETS_H

#include <Arduino.h>

// Packets declaraction for rocket side system
// Any changes here will NOT affect the ground side packets.h file!

// Establishing key numerical constants
constexpr uint8_t NUM_SERVO = 4;
constexpr uint8_t NUM_PT = 3;
constexpr uint8_t NUM_EMATCH = 2;

// Makes sure there are no garbage values inserted in the structs
#pragma pack(push,1)

// Initialization of Telemetry Packet
// Version Number: 2 (Change this if anything changes to keep track of discrepancies)
struct TelemetryPacket 
{
  uint32_t pressure[NUM_PT]; // Pressure measurements (psi) *100 (lessens storage space and keeps 2 decimal places)
  uint32_t loadCell; // Load cell measurement (lb) *100 (lessens storage space and keeps 2 decimal places)
  uint8_t servoPos[NUM_SERVO];
  bool solenoidState;
  uint32_t timestamp;
};

// Initialization of Command Packet
// Version Number: 2 (Change this if anything changes to keep track of discrepancies)
struct CommandPacket
{
  uint16_t packetID;
  bool servoState[NUM_SERVO];
  bool solenoidState;
  bool armedState;
  bool ematchState[NUM_EMATCH];
  bool tareLoadCellState;
  uint16_t crc;
};

#pragma pack(pop)

#endif