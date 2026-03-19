#include "actuators.h"


// initActuators() Function
// Initializes all actuators on the rocket
// Called within main setup
void initActuators(Servo servos[])
{
  // Attaching all servos to respective pins
  // Moving all servos to closed position
  // Detaching servos
  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    servos[i].attach(servoPins[i]);
    servos[i].write(servoClosed);
    delay(250);
    servos[i].detach();
  }

  // Attaching solenoid
  pinMode(solenoidPin,OUTPUT);

  // Attaching Ematches
  for (uint8_t i=0; i<NUM_EMATCH; i++)
  {
    pinMode(ematchPins[i],OUTPUT);
  }
}


// applyCommands() Function
// Moves all actuators to specified positions in the command packet
// Servo Motors are attached and detached to reduce twitching caused from RF signals
void applyActuateCommands(Servo servos[], bool servoAttachedState[], uint16_t servoActuationStart[], const CommandPacket &lastCmd, const CommandPacket &cmd)
{

  // Actuating all servo motors
  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    if (cmd.servoState[i] != lastCmd.servoState[i])
    {
      // Attaching servo if it was detached
      if (servoAttachedState[i] == false)
      {
        servos[i].attach(servoPins[i]); // Attaching servo
        servoAttachedState[i] = true; // Logging that servo is attached
      }

      // Writing commanded position to servo pin
      servos[i].write(cmd.servoState[i] ? servoOpen : servoClosed);

      // Updating time that servo started moving
      servoActuationStart[i] = millis();
    }
  }

  // Actuating solenoid
  if (cmd.solenoidState != lastCmd.solenoidState)
  {
    // Writing to solenoid pin
    digitalWrite(solenoidPin,cmd.solenoidState);
  }

  // Activating ematches
  for (uint8_t i=0; i<NUM_EMATCH; i++)
  {
    if (cmd.ematchState[i] != lastCmd.ematchState[i])
    { 
      if (cmd.armedState == 1 && cmd.ematchState[i] == 1)
      {
        digitalWrite(ematchPins[i],HIGH);
      }
      else
      {
        digitalWrite(ematchPins[i],LOW);
      }
    }
  }

}


// readActuatorStates() Function
// Reads actuator states into telemetry packet
void readActuatorStates(Servo servos[], TelemetryPacket &t)
{
  // Reading servo states
  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    t.servoPos[i] = servos[i].read();
  }

  // Reading solenoid state
  t.solenoidState = digitalRead(solenoidPin);
}


// updateServos() Function
// Detaches servo from PWM pin if enough time has passed
void updateServos(Servo servos[], bool servoAttachedState[], uint16_t servoActuationStart[])
{
  for(uint8_t i=0; i<NUM_SERVO; i++)
  {
    if (servoAttachedState[i] == true && millis() - servoActuationStart[i] > servoMoveTime)
    {
      servos[i].detach();
      servoAttachedState[i] = false;
    }
  }
}