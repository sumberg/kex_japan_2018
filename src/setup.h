#include <avr/io.h>
#include "ram.h"
#include "expander.h"
#include "SPI.h"

#define CTRL_DDR			DDRC
#define CTRL_PORT			PORTC
#define CTRL_RESET_SLAVE 	PC4

#define DEBUG_DDR 	DDRB
#define DEBUG_PORT 	PORTB
#define DEBUG_LED 	PB1

#define DATA_DDR 	DDRD
#define DATA_PORT 	PORTD
#define DATA_PIN 	PIND

#define TIMING_DDR		DDRC
#define TIMING_PORT		PORTC
#define TIMING_PORT_IN	PINC
#define READY_PIN		PC0
#define START_PIN		PC3
#define STOP_PIN		PC5

#define ERROR_DDR		DDRB
#define ERROR_PORT		PORTB
#define ERROR_LED		PB0

/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
