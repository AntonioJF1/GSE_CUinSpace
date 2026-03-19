#include "switches.h"

// -------------------------------
// initSwitches() Function
// -------------------------------
void initSwitches() {

  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    pinMode(Pin_Servos[i],INPUT_PULLUP);
  }
  pinMode(Pin_Solenoid,INPUT_PULLUP);
  for (uint8_t i=0; i<NUM_EMATCH; i++)
  {
    pinMode(Pin_Ematches[i],INPUT_PULLUP);
  }
  pinMode(Pin_Arm,INPUT_PULLUP);
  pinMode(Pin_AbortSwitch,INPUT_PULLUP);
  pinMode(Pin_LoadCellTare,INPUT_PULLUP);
  pinMode(Pin_SpareTare,INPUT_PULLUP);
}


// -------------------------------
// readSwitches() Function
// -------------------------------
void readSwitches(CommandPacket &cmd) {

  // Servo Commands
  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    cmd.servoState[i] = !digitalRead(Pin_Servos[i]);
  }

  // Solenoid Command
  cmd.solenoidState = !digitalRead(Pin_Solenoid);

  // Ematch Commands
  cmd.armedState = !digitalRead(Pin_Arm);
  for (uint8_t i=0; i<NUM_EMATCH; i++)
  {
    cmd.ematchState[i] = !digitalRead(Pin_Ematches[i]);
  }

  // Abort State
  if (!digitalRead(Pin_AbortSwitch)) {

    cmd.servoState[0] = false;
    cmd.servoState[1] = false;
    cmd.servoState[2] = false;
    cmd.servoState[3] = true;
    cmd.solenoidState = false;
    cmd.ematchState[0] = false;
    cmd.ematchState[0] = false;
  }

  // Tare Command
  if (!digitalRead(Pin_LoadCellTare)) {
    cmd.tareLoadCellState = true;
  }
  else
  {
    cmd.tareLoadCellState = false;
  }

}
