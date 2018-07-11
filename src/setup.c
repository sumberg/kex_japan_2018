#include "setup.h"

/* Setup all ports used on Atmega328 */
static void setup_ports(void)
{
	/* PORTD as output */
	DDRD = 0xFF;
	PORTD = 0x00;

	/* PB1 debug led */
	DDRB |= (1 << DEBUG_LED);

	// /********* CONTROL *********/
	// /*
	//  * PC1: RAM chip enable
	//  * PC2: RAM RW select
	//  * PC3: RAM OE select
	//  * PC4: Slave reset
	//  */
	DDRC = 0x1E;
}

void setup(void)
{
	SPI_setup();
	setup_ports();

	/* Set RAM to self refresh & read mode */
	RAM_CTRL_PORT |= ((1 << CE) | (1 << RW));

	/* Enable SPI in master mode (fck/2) */
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR |= (1 << SPI2X);

	SPDR = 0xFF;
	while (!(SPSR & (1 << SPIF)));

	/* Setup expander chip */
	expander_setup();
}
