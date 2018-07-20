#pragma once

#include <avr/io.h>

/* PORT direction bitmasks */
#define PORTB_SLAVE_RESET 0x01
#define PORTB_CLOCK_IN 0x00; // added for readability, should be zero on PB6
#define PORTC_ALL_OUTPUT 0xFF
#define PORTD_ALL_OUTPUT 0xFF

#define SLAVE_RESET_MASK 0xFE;
#define SLAVE_RELEASE_MASK 0x01;

#define DEBUG_PORT PORTC
#define DEBUG_LED PC4

/* Setup GPIO pins for input/output */
void setup_ports(void);
/* Sends (and holds) reset signal to slave */
void reset_slave(void);
/* Sends (and holds) release signal to slave. Used to undo reset/start slave. */
void release_slave(void);
/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
