#include <avr/io.h>

#include "expander.h"

/* Setup all ports used on Atmega328 */
void setup_ports(void);

void setup_ports(void)
{
	/********* SPI SETUP *********/
	/* Set MOSI, SCK, SS as output */
	DDRB = (1 << 5) | (1 << 3) | (1 << 2);;
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

	/* Debug LED on PB1 */
	DDRB |= (1 << 1);
}

void setup(void)
{
	/* Setup GPIO */
	setup_ports();

	/* Keep slave in reset mode */
	PORTC &= ~(1 << 4);

	/* RAM read mode as default */
	PORTC &= ~(1 << 2);

	/* Disable RAM during setup */
	PORTC |= (1 << 1);

	/* Enabe SPI in master mode */
	SPCR = (1 << SPE) | (1 << MSTR);

	/* Setup expander chip */
	setup_expander();

	/* Enable RAM */
	PORTC &= ~(1 << 1);

}
