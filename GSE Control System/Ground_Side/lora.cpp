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
// receiveTelemetry() Function
// -------------------------------
// Receiving telemetry from rocket
bool receiveTelemetry(RH_RF95 &LoRa, TelemetryPacket &t)
{
  // Calculating byte length of telemetry packet
  uint8_t len = sizeof(t);

  // Creating pointer to look at each individual byte in the struct
  // Casting the pointer to look at an array of individual bytes
  uint8_t* telemPtr = (uint8_t*)&t;

  // Checking if radio is transmitting
  if (LoRa.available())
  {
    // Receiving data
    if (LoRa.recv(telemPtr, &len))
    {
      if (len == sizeof(CommandPacket))
      {
        // DEBUG : printing what commands were received to serial monitor
        // *** Comment out normally for performance ***
        debugReceive(t);

        // Returning true if data of the correct length was received
        return true;
      }
    }
  }

  // Returning flase if no data was received
  return false;
}

// -------------------------------
// sendCommands() Function
// -------------------------------
// Sending commands to rocket
void sendCommands(RH_RF95 &LoRa, CommandPacket &cmd)
{
  // Calculating 
  cmd.crc = computeCRC16((uint8_t*)&cmd, sizeof(CommandPacket) - sizeof(uint16_t));
  
  // Sending commands
  LoRa.send((uint8_t*)&cmd, sizeof(cmd));

  // Waiting to receive ping back that the data was received
  LoRa.waitPacketSent();

  // DEBUG : printing what telemetry was sent to serial monitor
  // *** Comment out normally for performance ***
  debugSend(cmd);  
}

// -------------------------------
// debugReceive() Function
// -------------------------------
// Debugging function that prints out all received telemetry to serial monitor
void debugReceive(const TelemetryPacket &t)
{
  Serial.println("Telemetry Received:");
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

// -------------------------------
// debugSend() Function
// -------------------------------
// Debugging function that prints out all sent commands to serial monitor
void debugSend(const CommandPacket &cmd)
{
  Serial.println("Commands Sent:");
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



























