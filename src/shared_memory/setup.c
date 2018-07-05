#include <avr/io.h>

#include "expander.h"

/* Setup all ports used on Atmega328 */
void setup_ports(void);

void setup_ports(void)
{
	/********* SPI SETUP *********/
	/* Set MOSI, SCK as output */
	DDRB = (1 << 5) | (1 << 3);
	/* Data bus on PORTD */
	DDRD = 0xFF;

	/********* CONTROL *********/
	/*
	 * PC0: GPIO expander chip select
	 * PC1: RAM chip enable
	 * PC2: RAM RW select
	 * PC3: RAM OE select
	 * PC4: Slave reset
	 */
	DDRC = 0x1F;
}

void setup(void)
{
	setup_ports();

	/* Enabe SPI in master mode */
	SPCR = (1 << SPE) | (1 << MSTR);

	/* Setup expander chip */
	setup_expander();
}
