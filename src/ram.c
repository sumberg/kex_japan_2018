#define F_CPU 20000000UL
#include <util/delay.h>
#include "ram.h"

/* Outputs the address that will be used on RAM chip from expander ports */
/* Params:
 * 		mem_hi - High 8 bits of 16-bit address (Expander PORTA)
 * 		mem_lo - Low 8 bits of 16-bit address (Expander PORTB)
 * */
static void set_RAM_address(uint8_t mem_hi, uint8_t mem_lo)
{
	/* Set mem_hi on PORTA, mem_lo on PORTB */
	expander_set_ports_AB(mem_hi, mem_lo);
}

/* Performs a single byte write to RAM
 * Params:
 * 		mem_hi - High 8 bits of 16-bit address
 * 		mem_lo - Low 8 bits of 16-bit address
 * 		data - One byte of data to be written to RAM
 **/
void RAM_write(uint8_t mem_hi, uint8_t mem_lo, uint8_t data)
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

/* Performs a single byte read from RAM
 * Params:
 * 		mem_hi - High 8 bits of 16-bit address
 * 		mem_lo - Low 8 bits of 16-bit address
 **/
uint8_t RAM_read(uint8_t mem_hi, uint8_t mem_lo)
{
	uint8_t data;

	/* Set Data port as input temporarily to perform read */
	DDRD = 0x00;

	/* Set read address, and hold until data is stable (at least 55ns) */
	set_RAM_address(mem_hi, mem_lo);
	_delay_us(1);

	/* Read data */
	data = PIND;
	
	/* Reset Data port to output mode, and return read data */
	DDRD = 0xFF;

	return data;
}
