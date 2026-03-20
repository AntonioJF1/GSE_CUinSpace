// LoRa Transmitter Test
#include <SPI.h>
#include <RH_RF95.h>

// Establishing radio ports and variables
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0

// Establishing radio object
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Packet to send over 
float sensorValue = 0.0;

void setup() {
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST,HIGH);

  Serial.begin(9600);
  delay(1000);

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  SPI.begin();

  if(!rf95.init()) {
    Serial.println("LoRa Initialization Failed");
    while(1);
  }

  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);

  Serial.println("LoRa Float Transmitter Ready");
}

void loop() {

  // Printing to serial monitor
  Serial.print("Sending float: ");
  Serial.println(sensorValue);

  // Sending signal
  rf95.send((uint8_t*)&sensorValue, sizeof(sensorValue));
  rf95.waitPacketSent();

  // Incrementing sensor value
  sensorValue = sensorValue + 0.3f;

  // Delaying 1 second
  delay(1000);
}
