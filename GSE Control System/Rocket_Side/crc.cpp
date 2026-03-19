#include "crc.h"

// -------------------------------
// computeCRC16() Function
// -------------------------------
// Implements CRC-16-CCITT (0x1021)
// Creates a unique 16 bit code based on the contents of the struct
// Makes sure data wasn't changed from trasmission to receiving the data
// If one bit changes, the crc changes (creates number from bit manipulations)
// It is ESSENTIAL that this code is identical to that on rocket side
uint16_t computeCRC16(const uint8_t* data, uint16_t length)
{
  // Initializing crc number
  uint16_t crc = 0xFFFF;

  // Cycling through each byte in the data
  for (uint16_t i=0; i<length; i++)
  {
    // (uint16_t)data[i]: typecasts a byte (8 bits) into 16 bit (00000000xxxxxxxx)
    // << 8: shifts byte 8 bits to the left (xxxxxxxx00000000)
    // ^=: exclusive or gate (XOR)
    crc ^= (uint16_t)data[i] << 8;

    // Processing each bit individually
    for (uint8_t j=0; j<8; j++)
    {
      // Checking if this is the first bit
      // &: and gate (AND)
      // 0x8000: corresponsds to first bit in 16 bit (1000000000000000)
      if (crc & 0x8000)
      {
        // crc << 1: shifts bits 1 to the left (multiply by two bitwise)
        // ^ 0x1021: XOR with a constant (CRC polynomial)
        crc = (crc << 1) ^ 0x1021;
      }
      else
      {
        // crc <<= 1: just shifts bits 1 to the left
        crc <<= 1;
      }
    }
  }

  // Returning crc calculated value
  return crc;
}