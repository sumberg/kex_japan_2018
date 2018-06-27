#include <avr/io.h>
#include <util/delay.h>

#define OPCODE_WRITE 0x40
#define OPCODE_READ 0x41
#define TRANSFER_COMPLETE (SPSR & (1 << SPIF))

/* Puts the RAM address to be accessed on gpio expander chip ports */
void set_RAM_address(uint8_t, uint8_t);
/* Performs a write to memory */
void write_RAM(uint8_t, uint8_t, uint8_t);

void set_RAM_address(uint8_t mem_hi, uint8_t mem_lo)
{
	/* Initiate expander chip communication with chip select (low on P0) */
	PORTC &= ~0x01;

	/* Write expander chip opcode, followed by start address for PORTA */
	SPDR = OPCODE_WRITE;
	while(!TRANSFER_COMPLETE);
	SPDR = 0x12;

	/* Place mem_hi on expander chip PORTB */
	SPDR = mem_hi;
	while(!TRANSFER_COMPLETE);
	SPDR = mem_lo;
	while(!TRANSFER_COMPLETE);

	/* End communication by deselecting expander chip */
	PORTC |= 0x01;
}

void write_RAM(uint8_t mem_hi, uint8_t mem_lo, uint8_t data)
{
	set_RAM_address(mem_hi, mem_lo);

	/* Prepare data on bus */
	PORTD = data;

	/* Enable RAM write, and hold value until written (at least 55 ns) */
	PORTC &= ~(1 << 2);
	_delay_us(10);

	/* Disable RAM write */
	PORTC |= (1 << 2);
}
