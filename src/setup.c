#define F_CPU 20000000UL
#include <util/delay.h>
#include "setup.h"
#include "2a03.h"

/* Set up the gpio pins for input/output etc. */
void setup_ports(void)
{
	/* PORTD - Data output pins */
	/* Using entire 8 bits of register as output */
	DDRD = PORTD_ALL_OUTPUT;
	/* PORTB - External clock and slave reset control, Timer stop output PB3 */
	DDRB = PORTB_SLAVE_RESET | PORTB_CLOCK_IN | (3 << PB3);
	/* PC0-PC2 data input, PC3 Latch OE, PC4-PC5 debug leds */
	DDRC = (7 << 3);
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

/* Set up all interrupts that will be used on the Atmega328 */
void setup_master_interrupts(void)
{
	/* Enable global interrupts */
	sei();

	/* TC0 in CTC mode */
	TCCR0A = 0b10 << WGM00;

	/* Use full period register, and count overflows */
	OCR0A = 255;

	/* Enable compare match interrupt */
	TIMSK0 = 1 << OCIE0A;

	/* Start timer as CTC, prescaler 8 */
	TCCR0B = (1 << FOC0A) | (0b010 << CS00);
}

ISR(TIMER0_COMPA_vect)
{
	static int overflowCount = 0;
	if (overflowCount >= globalTimerOverflowTimeout) {
		reset_slave_pc();
		overflowCount = 0;
	}
}

/* Run entire setup routine */
void setup(void)
{
	setup_ports();

	/* Perform slave reset cycle */
	reset_slave();
	_delay_ms(1000);
	release_slave();
	_delay_us(1000);

	/* Figure out slave divider, disable slave interrupts and reset slave pc */
	setup_slave_timing();

	/* Master interrupt timing and routines */
	setup_master_interrupts();
}
