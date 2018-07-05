#include <util/delay.h>
#include "expander.h"

/* Puts the RAM address to be accessed on gpio expander chip ports */
void set_RAM_address(uint8_t, uint8_t);
/* Performs a write to memory */
void write_RAM(uint8_t, uint8_t, uint8_t);

static void set_RAM_address(uint8_t mem_hi, uint8_t mem_lo)
{
	/* Set mem_hi on PORTA, mem_lo on PORTB */
	expander_set_ports_AB(mem_hi, mem_lo);
}

void write_RAM(uint8_t mem_hi, uint8_t mem_lo, uint8_t data)
{
	set_RAM_address(mem_hi, mem_lo);

	/* Prepare data on bus */
	PORTD = data;

	/* Enable RAM write, and hold value until written (at least 55 ns) */
	PORTC &= ~(1 << 2);
	_delay_us(1);

	/* Disable RAM write */
	PORTC |= (1 << 2);
}
