#ifndef CRC_H
#define CRC_H

#include <stdint.h>

// Implements CRC-16-CCITT (0x1021)
// Creates a unique 16 bit code based on the contents of the struct
// Makes sure data wasn't changed from trasmission to receiving the data
// If one bit changes, the crc changes (creates number from bit manipulations)
// It is ESSENTIAL that this code is identical to that on rocket side
uint16_t computeCRC16(const uint8_t* data, uint16_t length);

#endif