// LoRa Receiver Test w/ Seven Segment Display
#include <SPI.h>
#include <RH_RF95.h>

// Establishing radio ports and variables
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0

// Establishing radio object
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Initializing value to be sent
float value;

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
}

void loop() {
  
  float lastValue = 0;
  
  if (rf95.available())
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
       float receivedValue;
       memcpy(&receivedValue, buf, sizeof(receivedValue));
       
       lastValue = receivedValue;

       Serial.print("Received: ");
       Serial.println(receivedValue);
    }    
  }

  delay(10);
}

