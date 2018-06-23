#define F_CPU 20000000
#include <util/delay.h>
#include "setup.h"
#include "2a03.h"

/* Set up the gpio pins for input/output etc. */
void setup_ports(void)
{
	/* PORTD - Data output pins */
	/* Using entire 8 bits of register as output */
	DDRD = PORTD_ALL_OUTPUT;
	/* PORTB - External clock and slave reset control */
	DDRB =
		PORTB_SLAVE_RESET
		| PORTB_LATCH_OE
		| PORTB_CLOCK_IN;
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

/* Enable latch output */
void latch_output_enable(void)
{
	/* Latch OE is inverted, make low to enable */
	PORTB &= LATCH_OUTPUT_ENABLE_MASK;
}

/* Disable latch output (tri-state) */
void latch_output_disable(void)
{
	/* Latch OE is inverted, make high to disable */
	PORTB |= LATCH_OUTPUT_DISABLE_MASK;
}

/* Set up all interrupts that will be used on the Atmega328 */
void setup_master_interrupts(void)
{

	/* Enable global interrupts */
	sei();

	/* TC0 in CTC mode */
	TCCR0A = 0b10 << WGM00;

	/* Set period */
	OCR0A = 244;

	/* Enable compare match interrupt */
	TIMSK0 = 1 << OCIE0A;

	/* Start timer as CTC, prescaler 1024 */
	TCCR0B = (1 << FOC0A) | (0b101 << CS00);
}

ISR(TIMER0_COMPA_vect)
{
	reset_slave_pc();
}

/* Run entire setup routine */
void setup(void)
{
	setup_ports();

	latch_output_enable();
	/* Perform slave reset cycle */
	reset_slave();
	_delay_ms(1000);
	release_slave();
	_delay_us(1000);

	setup_slave_timing();

	/* Master interrupt timing and routines */
	setup_master_interrupts();
}
