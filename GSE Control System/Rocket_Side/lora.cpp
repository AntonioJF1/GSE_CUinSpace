#include "lora.h"

// -------------------------------
// initLoRa() Function
// -------------------------------
// Initializes LoRa Transceiver
void initLoRa(RH_RF95 &LoRa)
{
  // Manually restarting LoRa
  // Basically power-cycling the radio
  pinMode(LoRa_RST, OUTPUT);
  digitalWrite(LoRa_RST, LOW);
  delay(10);
  digitalWrite(LoRa_RST, HIGH);
  delay(10);

  // Checking LoRa Initialization
  if (!LoRa.init())
  {
    Serial.println("LoRa Initialization Failed");
    while (1);
  }

  // Setting radio parameters
  LoRa.setFrequency(LoRa_Freq);
  LoRa.setTxPower(23, false);
}

// -------------------------------
// receiveCommands() Function
// -------------------------------
// Receiving commands from ground station
bool receiveCommands(RH_RF95 &LoRa, CommandPacket &cmd, CommandPacket &lastCmd)
{
  // Temporary storage
  CommandPacket temp;
  
  // Calculating byte length of command packet
  uint8_t len = sizeof(cmd);

  // Creating pointer to look at each individual byte in the struct
  // Casting the pointer to look at an array of individual bytes
  uint8_t* cmdPtr = (uint8_t*)&temp;

  // Checking if other radio is transmitting
  if (LoRa.available())
  {
    // Receiving data
    if(LoRa.recv(cmdPtr, &len))
    {
      // Grabbing crc that was sent
      uint16_t sentCRC = temp.crc;

      // Removing crc from struct (will interfere with calculations)
      temp.crc = 0;

      // Computing crc from received struct (sent crc not included)
      uint16_t computedCRC = computeCRC16((uint8_t*)&temp, sizeof(CommandPacket) - sizeof(temp.crc));

      if (sentCRC == computedCRC)
      {
        // Storing structs
        lastCmd = cmd;
        cmd = temp;

        // DEBUG : printing what commands were received to serial monitor
        // *** Comment out normally for performance ***
        debugReceive(cmd);

        // Returning true if data of correct size was received
        return true;
      }
    }
  }

  // Returning false if no data was received or crc failed
  return false;
}

// -------------------------------
// sendTelemetry() Function
// -------------------------------
// Sending telemetry to ground station (sensor data and actuator position)
void sendTelemetry(RH_RF95 &LoRa, const TelemetryPacket &t)
{
  // Sending telemetry
  LoRa.send((uint8_t*)&t, sizeof(t));

  // Waiting to receive ping back that the data was received
  LoRa.waitPacketSent();

  // DEBUG : printing what telemetry was sent to serial monitor
  // *** Comment out normally for performance ***
  debugSend(t);
}

// -------------------------------
// debugReceive() Function
// -------------------------------
// Debugging function that prints out all received commands to serial monitor
void debugReceive(const CommandPacket &cmd)
{
  Serial.println("Commands Received:");
  Serial.print("packetID: ");
  Serial.print(cmd.packetID);
  Serial.print(" | Servo States: ");
  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    Serial.print(cmd.servoState[i]);
    Serial.print(",");
  }
  Serial.print(" | Solenoid State: ");
  Serial.print(cmd.solenoidState);
  Serial.print(" | Armed State: ");
  Serial.print(cmd.armedState);
  Serial.print(" | Ematch States: ");
  for (uint8_t i=0; i<NUM_EMATCH; i++)
  {
    Serial.print(cmd.ematchState[i]);
  }
  Serial.print(" | Tare Load Cell State: ");
  Serial.print(cmd.tareLoadCellState);
  Serial.print(" | CRC: ");
  Serial.println(cmd.crc);
}

// -------------------------------
// debugSend() Function
// -------------------------------
// Debugging function that prints out all sent telemetry to serial monitor
void debugSend(const TelemetryPacket &t)
{
  Serial.println("Telemetry Sent:");
  Serial.print("Pressures: ");
  for (uint8_t i=0; i<NUM_PT; i++)
  {
    Serial.print(t.pressure[i]);
    Serial.print(",");
  }
  Serial.print(" | Load Cell: ");
  Serial.print(t.loadCell);
  Serial.print(" | Servo Positions: ");
  for (uint8_t i=0; i<NUM_SERVO; i++)
  {
    Serial.print(t.servoPos[i]);
    Serial.print(",");
  }
  Serial.print(" | Solenoid State: ");
  Serial.print(t.solenoidState);
  Serial.print(" | Timestamp: ");
  Serial.println(t.timestamp);
}







