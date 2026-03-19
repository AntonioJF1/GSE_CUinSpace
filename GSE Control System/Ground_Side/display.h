#ifndef DISPLAY_H
#define DISPLAY_H

#include "packets.h"
#include "config.h"
#include <Wire.h> // Controls I2C Bus (Needed for LCD screens)
#include <LiquidCrystal_I2C.h> // Controls the I2C communication with the LCD Screens (*** Need to Install LiquidCrystal_I2C by Martin Kubovcik)
#include <LedControl.h> // Controls the 7 Segment Displays (*** Need to Download LedControl by Eberhard Fahle)

// Key LCD Variables
constexpr uint16_t LCD_Address[NUM_LCD] = {0x20,0x21};

void initDisplays(LiquidCrystal_I2C LCDs[], LedControl &SevSD);

void updateDisplays(const TelemetryPacket &t);

void displayNumber7SD(uint8_t device, long value);

#endif