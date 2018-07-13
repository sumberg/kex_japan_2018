#pragma once

#include "expander.h"

#define RAM_CTRL_DDR	DDRC
#define RAM_CTRL_PORT	PORTC
#define RAM_OE			PC0
#define RAM_CE			PC1
#define RAM_RW			PC2

/* Setup control pins for RAM */
void RAM_setup(void);
/* Perform a write to memory */
void RAM_write(uint8_t, uint8_t, uint8_t);
/* Perform a read from memory */
uint8_t RAM_read(uint8_t, uint8_t);
