#ifndef SWITCHES_H
#define SWITCHES_H

#include "packets.h"
#include "config.h"

void initSwitches();

void readSwitches(CommandPacket &command);

#endif
