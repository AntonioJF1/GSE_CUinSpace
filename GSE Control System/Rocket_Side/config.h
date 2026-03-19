#ifndef CONFIG_H
#define CONFIG_H

#include "packets.h"

// -------------------------------
// LoRa Configuration
// -------------------------------
// G0(INT)->2, RST->9, CS->10, MOSI->51, MISO->50, SCK->52
constexpr uint8_t LoRa_INT = 2;
constexpr uint8_t LoRa_RST = 9;
constexpr uint8_t LoRa_CS = 10;

// -------------------------------
// Actuator Configurations
// -------------------------------
// Servo1 = Fill -> 3
// Servo2 = Tank -> 4
// Servo3 = Vent -> 5
// Servo4 = Dump -> 6
// Solenoid -> 8
// Ematch1 -> 31
// Ematch2 -> 33
const uint8_t servoPins[NUM_SERVO] = {3,4,5,6};
constexpr uint8_t solenoidPin = 8;
constexpr uint8_t ematchPins[NUM_EMATCH] = {31,33};

// -------------------------------
// Sensor Configurations
// -------------------------------
// PT1 -> A0
// PT2 -> A1
// PT3 -> A2
// Load Cell: DAT -> 11, CLK -> 7
const uint8_t pressurePins[NUM_PT] = {A0,A1,A2};
constexpr uint8_t loadCell_DT = 11;
constexpr uint8_t loadCell_CLK = 7;

#endif