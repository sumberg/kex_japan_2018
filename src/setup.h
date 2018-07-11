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

/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
