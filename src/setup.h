#include <avr/io.h>
#include "ram.h"
#include "expander.h"
#include "SPI.h"

#define DEBUG_LED 0x01

/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
