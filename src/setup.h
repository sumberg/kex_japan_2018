#pragma once

#include <avr/io.h>

/* PORT direction bitmasks */
#define PORTB_SLAVE_RESET 0x01
#define PORTB_CLOCK_IN 0x00		// added for readability, should be zero on PB6
#define PORTC_ALL_OUTPUT 0xFF
#define PORTD_ALL_OUTPUT 0xFF

#define SLAVE_RESET_MASK 0xFE;
#define SLAVE_RELEASE_MASK 0x01;

#define DEBUG_PORT PORTC
#define DEBUG_LED PC4
#define DONE_LED PC5

#define TIMING_PORT PORTB
#define TIMING_PORT_IN PINB
#define START_SLAVE_EXEC PB2
#define STOP_SLAVE_EXEC PB3
#define SETUP_DONE_PIN PB4

#define TIMER_OVERFLOW_MAX 180		// With prescaler set to 8, 180 overflows yields ~54Hz
#define TIMER_OVERFLOW_DEFAULT 122	// With prescaler set to 8, 122 overflows yields ~80Hz
#define TIMER_OVERFLOW_MIN 40		// With prescaler set to 8, 48 overflows yields ~200Hz

extern int globalTimerOverflowTimeout;

/* Setup GPIO pins for input/output */
void setup_ports(void);
/* Sends (and holds) reset signal to slave */
void reset_slave(void);
/* Sends (and holds) release signal to slave. Used to undo reset/start slave. */
void release_slave(void);
/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
