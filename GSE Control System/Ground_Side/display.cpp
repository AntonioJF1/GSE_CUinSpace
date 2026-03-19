#include "display.h"

// **************NOT COMPLETED**********************
void initDisplays(LiquidCrystal_I2C LCDs[], LedControl &SevSD)
{
  // 7SD Setup (All 4 in Series)
  for (int i=0; i<NUM_SevSD; i++) {
  SevSD.shutdown(i,false);
  SevSD.setIntensity(i,5);
  SevSD.clearDisplay(i);
  }
}

// **************NOT COMPLETED**********************
// -------------------------------
// updateDisplays()
// -------------------------------
void updateDisplays(TelemetryPacket &t) {

  // Only updating every 1/10th of a second
  // if(millis() - lastDisplayUpdate < 100) return;

  // Updating last update variable to current time
  // lastDisplayUpdate = millis();

  // Displaying the values to each 7SD
  // displayNumber7SD(0,(long)(t.pressure[0]));
  // displayNumber7SD(1,(long)(t.pressure[1]));
  // displayNumber7SD(2,(long)(t.pressure[2]));
  // displayNumber7SD(3,(long)(t.loadCell));
}

void displayNumber7SD(uint8_t device, long value)
{

}



