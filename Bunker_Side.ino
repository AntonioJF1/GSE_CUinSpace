// Arduino Mega Code for Bunker Side GSE System
// Contributors: Antonio Jimeno-Fernandez
// Date Modified: 3/12/26

// -------------------------------
// Bunker Side System Overview:
// -------------------------------
// - 1 Arduino Mega
// - 1 915 MHz LoRa Transceiver
// - 4 Max7219 Seven-Segment Displays
// - 2 lcd1602 I2C Liquid Crystal Displays
// - 7 Switches

// -------------------------------
// Including Needed Libraries
// -------------------------------
#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>
#include <LedControl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------------------------------
// LoRa Configuration
// -------------------------------
// G0(INT)->2, RST->9, CS->10, MOSI->51, MISO->50, SCK->52
#define LoRa_INT 2
#define LoRa_RST 9
#define LoRa_CS 10
#define LoRa_Freq 915.0
// Initializing LoRa Radio Object
RH_RF95 LoRa(LoRa_CS, LoRa_INT); 

// -------------------------------
// Define Switches
// -------------------------------
#define switch1 37
#define switch2 38
#define switch3 39
#define switch4 40
#define switch5 41
#define switch6 42
#define switch7 43
#define switch8 44
#define button1 45
#define button2 46

// -------------------------------
// Creating Telemetry Packet Structure
// -------------------------------
struct TelemetryPacket{
  float pressure1;
  float pressure2;
  float pressure3;
  float loadCell;
  int servo1Pos;
  int servo2Pos;
  int servo3Pos;
  int servo4Pos;
  bool solenoidState;
  unsigned long timestamp;
};
// Initializing telemetry packet object
TelemetryPacket telemetry;


// -------------------------------
// Creating Command Packet Structure
// -------------------------------
struct CommandPacket{
  uint16_t packetID; // Ignores duplicate packets (increments each packet)
  bool servo1State;
  bool servo2State;
  bool servo3State;
  bool servo4State;
  bool solenoidState;
  uint16_t crc; // Ensures packet is not corrupted
};
// Initializing command packet object
CommandPacket command;


// -------------------------------
// Display Setup
// -------------------------------
// Defining 7 Segment Displays
// DIN, CLK, CS, number of MAX7219 chips
LedControl SevSD(23, 24, 25, 4);

// Defining LCDs
// LCD address, columns, rows
LiquidCrystal_I2C lcd1(0x20, 16, 2);
LiquidCrystal_I2C lcd2(0x21, 16, 2);
// I2C backpack allows multiple LCDs to be used from the same two data and clk pins.
// I2C uses fixed pins for Mega. SDA: 20, SCL: 21
// Address is set by soldering closed certain pins on the backpack. 0x20 default, A0 soldered for 0x21.


void setup() { 

  Serial.begin(9600); 
  setupLora();

  // Wake up MAX7219s
   SevSD.shutdown(0,false);

  // Brightness (0-15)
  int brightness = 3;
  SevSD.setIntensity(0,brightness);

  // Clear displays
  SevSD.clearDisplay(0);

  // Initialize LCDs
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();

  lcd1.setCursor(0,0);
  lcd1.print("Display 1");
  lcd2.setCursor(0,0);
  lcd2.print("Display 2");

  // Configure Pins
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(switch5, INPUT_PULLUP);
  pinMode(switch6, INPUT_PULLUP);
  pinMode(switch7, INPUT_PULLUP);
  pinMode(switch8, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

} 

void loop() {   

  if (receiveTelemetry(telemetry)) 
  {
    // Display to 7SDs (display number 1-4, value)
    displayNumber(0, (long)(telemetry.pressure1 * 100));
    displayNumber(1, (long)(telemetry.pressure2 * 100));
    displayNumber(2, (long)(telemetry.pressure3 * 100));
    displayNumber(3, (long)(telemetry.loadCell * 100));

    // Display to LCDs
    lcd1.setCursor(0,0);  // Column, row
    lcd1.print("Load Cell [lbf]:");
    lcd1.setCursor(0,1);
    lcd1.print("                "); // clears row
    lcd1.setCursor(0,1);
    lcd1.print(telemetry.loadCell, 2); // Value, decimal places

    lcd2.setCursor(0,0);
    lcd2.print("Load Cell [lbf]:");
    lcd2.setCursor(0,1);
    lcd2.print("                ");
    lcd2.setCursor(0,1);
    lcd2.print(telemetry.loadCell, 2);
  }

  // Create Command Packet
  command.servo1State = digitalRead(switch1);
  command.servo2State = digitalRead(switch2);
  command.servo3State = digitalRead(switch3);
  command.servo4State = digitalRead(switch4);
  command.solenoidState = digitalRead(switch5);
  
  // = digitalRead(switch6);
  // abort = digitalRead(switch7);
  command.packetID++;
  command.crc = 0;

  // Send Command States over LoRa
  sendCommand(command);

  // Delaying 50 ms 
  delay(50); 
} 

// -------------------------------
// setup LoRa
// -------------------------------
void setupLora(){
  
  // Cycling through and reseting transceiver
  pinMode(LoRa_RST, OUTPUT);
  digitalWrite(LoRa_RST, HIGH);
  delay(10);
  digitalWrite(LoRa_RST, LOW);
  delay(10);
  digitalWrite(LoRa_RST, HIGH);
  delay(10);

  // Checking Initialization
  if (!LoRa.init()) {
    Serial.println("LoRa Initialization Failed");
    while(1);
  }

  // Checking Frequency Setting
  if (!LoRa.setFrequency(LoRa_Freq)) {
    Serial.println("LoRa Set Frequency Failed");
    while(1);
  }

  // Setting Output Power
  LoRa.setTxPower(23, false);

  // Printing System is Ready
  Serial.println("LoRa Ready");
}


// -------------------------------
// Receive Telemetry
// -------------------------------
bool receiveTelemetry(TelemetryPacket &packet)
{
  if (LoRa.available())
  {
    Serial.println("Packet Received");
    uint8_t buf[sizeof(TelemetryPacket)];
    uint8_t len = sizeof(buf);

    if (LoRa.recv(buf, &len))
    {
      if (len == sizeof(TelemetryPacket))
      {
        memcpy(&packet, buf, sizeof(TelemetryPacket));
        return true;
      }
    }
  }
  return false;
}

void sendCommand(CommandPacket &packet)
{
  LoRa.send((uint8_t*)&packet, sizeof(packet));
  LoRa.waitPacketSent();
}


// -------------------------------
// Display to 7 Segment Display
// -------------------------------
void displayNumber(int display, long number){

    for (int i=0; i<8; i++) {

      int digit = number % 10;
      bool decimal = (i==2);

      if (display == 1) {
        SevSD.setDigit(0,i,digit,decimal);
      } else if (display == 2) {
        SevSD.setDigit(1,i,digit,decimal);
      } else if (display == 3) {
        SevSD.setDigit(2,i,digit,decimal);
      } else {
        SevSD.setDigit(3,i,digit,decimal);
      }

      number /= 10;

      if (number == 0) break;
    }
}