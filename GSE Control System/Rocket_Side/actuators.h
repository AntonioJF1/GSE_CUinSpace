#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "packets.h"
#include "config.h"
#include <Servo.h>

// -------------------------------
// Key Actuator Variables
// -------------------------------
constexpr uint8_t servoClosed = 65;
constexpr uint8_t servoOpen = 0;
constexpr uint8_t servoMoveTime = 250; // Time to when servo can finish movement


// -------------------------------
// Function Definitions
// -------------------------------

// Initializes all actuators
// (Servos, Solenoid, Ematches, Failsafes)
void initActuators(Servo servos[]);

// Applies the commands to the actuators
void applyActuateCommands(Servo servos[], bool servoAttachedState[], uint16_t servoActuationStart[], const CommandPacket &lastCmd, const CommandPacket &cmd);

// Reads actuator states into telemetry packet
void readActuatorStates(Servo servos[], TelemetryPacket &t);

// Detaches servo from PWM pin if enough time has passed
void updateServos(Servo servos[], bool servoAttachedState[], uint16_t servoActuationStart[]);


#endif