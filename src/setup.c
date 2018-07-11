#include "setup.h"

/* Setup all ports used on Atmega328 */
static void setup_ports(void)
{
	/* PORTD as output */
	DATA_DDR = 0xFF;
	DATA_PORT = 0x00;

	/* PB1 debug led */
	DEBUG_DDR |= (1 << DEBUG_LED);

	/* Slave reset control as output */
	CTRL_DDR |= (1 << CTRL_RESET_SLAVE);
	/* Keep slave in reset mode */
	CTRL_PORT &= ~(1 << CTRL_RESET_SLAVE);
}

void setup(void)
{
	SPI_setup();
	RAM_setup();
	setup_ports();

	/* Setup expander chip */
	expander_setup();
}
