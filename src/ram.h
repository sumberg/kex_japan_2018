#pragma once

#include "expander.h"

/* Perform a write to memory */
void RAM_write(uint8_t, uint8_t, uint8_t);
/* Perform a read from memory */
uint8_t RAM_read(uint8_t, uint8_t);
