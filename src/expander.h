#pragma once

#include <stdint.h>
#include "SPI.h"

/* Initializes ports as output, and other settings */
void expander_setup(void);
/* Write data to expander address addr */
void expander_write(uint8_t address, uint8_t data);
/* Sets value to output on port A and B */
void expander_set_ports_AB(uint8_t dataA, uint8_t dataB);
