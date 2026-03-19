#ifndef CONFIG_H
#define CONFIG_H

// -------------------------------
// LoRa Configuration
// -------------------------------
// G0(INT)->2, RST->9, CS->10, MOSI->51, MISO->50, SCK->52
constexpr uint8_t LoRa_INT = 2;
constexpr uint8_t LoRa_RST = 9;
constexpr uint8_t LoRa_CS = 10;

// -------------------------------
// 7 Segment Display Configurations
// -------------------------------
// All seven segments are connected in series and are treated as a singular display
constexpr uint8_t SevSD_DIN = 23;
constexpr uint8_t SevSD_CS = 24;
constexpr uint8_t SevSD_CLK = 25;


// -------------------------------
// LCD Screen Configurations
// -------------------------------
constexpr uint8_t LCD_SDA = 20;
constexpr uint8_t LCD_SCK = 21;

// -------------------------------
// Switch and Button Inputs
// -------------------------------
constexpr uint8_t Pin_Servos[NUM_SERVO] = {30,31,32,33};
constexpr uint8_t Pin_Solenoid = 34;
constexpr uint8_t Pin_Ematches[NUM_EMATCH] = {35,36};
constexpr uint8_t Pin_Arm = 37;
constexpr uint8_t Pin_AbortSwitch = 38;
constexpr uint8_t Pin_LoadCellTare = 39;
constexpr uint8_t Pin_SpareTare = 40;

#endif