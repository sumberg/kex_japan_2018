#pragma once

#include <stdint.h>
#include <avr/io.h>

#define TRANSFER_COMPLETE (SPSR & (1 << SPIF))

#define EXPANDER_OPCODE_WRITE 0x40
#define EXPANDER_OPCODE_READ 0x41
#define EXPANDER_DIRA 0x00
#define EXPANDER_DIRB 0x01
#define EXPANDER_PORTA 0x12
#define EXPANDER_PORTB 0x13

#define CS 0x02

/* Initializes ports as output, and other settings */
void expander_setup(void);
/* Write data to expander address addr */
void expander_write(uint8_t address, uint8_t data);
/* Sets value to output on port A and B */
void expander_set_ports_AB(uint8_t dataA, uint8_t dataB);
