#include "setup.h"

/* Set up the gpio pins for input/output etc. */
void setup_ports(void)
{
	/* PORTD - Data pins */
	/* Using entire 8 bits of register as output */
	DDRD = PORTD_ALL_OUTPUT;
	/* PORTB - External clock and slave reset control */
	/* PB6 = 0 for clock input, PB1 = 0 for slave R/W scanning
	 * and PB0 = 1 for reset control output */
	DDRB = PORTB_SLAVE_RESET;
}

/* Sends (and holds) reset signal to slave */
void reset_slave(void)
{
	PORTB &= SLAVE_RESET_MASK;
}

/* Sends (and holds) release signal to slave. Used to undo reset/start slave. */
void release_slave(void)
{
	PORTB |= SLAVE_RELEASE_MASK;
}

/* Run entire setup routine, and leave slave in reset mode */
void setup(void)
{
	setup_ports();
	reset_slave();
}
