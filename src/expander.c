#include "expander.h"

void setup_expander(void)
{
	uint8_t tmp;

	/* Send opcode */
	SPDR = EXPANDER_OPCODE_WRITE;
	while(!TRANSFER_COMPLETE);
	tmp = SPDR;

	/* Set PORTA and PORTB as input */
	SPDR = EXPANDER_DIRA;
	while(!TRANSFER_COMPLETE);
	tmp = SPDR;

	SPDR = 0x00;
	while(!TRANSFER_COMPLETE);
	tmp = SPDR;

	SPDR = EXPANDER_DIRB;
	while(!TRANSFER_COMPLETE);
	tmp = SPDR;

	SPDR = 0x00;
	while(!TRANSFER_COMPLETE);
	tmp = SPDR;

	/* Deactivate communication (CS->HIGH) */
	PORTC |= (1 << CS);
}

/* Write data to expander address addr */
void expander_write(uint8_t addr, uint8_t data)
{
	/* Activate communication, and send opcode */
	PORTC &= ~(1 << CS);
	SPDR = EXPANDER_OPCODE_WRITE;
	while(!TRANSFER_COMPLETE);

	/* Write address */
	SPDR = addr;
	while(!TRANSFER_COMPLETE);

	/* Write data */
	SPDR = data;
	while(!TRANSFER_COMPLETE);

	/* End communication */
	PORTC |= (1 << CS);
}

void expander_set_ports_AB(uint8_t dataA, uint8_t dataB)
{
	/* Activate communication, and send opcode */
	PORTC &= ~(1 << CS);
	SPDR = EXPANDER_OPCODE_WRITE;
	while(!TRANSFER_COMPLETE);

	/* Write PORTA address */
	SPDR = EXPANDER_PORTA;
	while(!TRANSFER_COMPLETE);

	/* Write PORTA data */
	SPDR = dataA;
	while(!TRANSFER_COMPLETE);

	/* Write PORTB data */
	SPDR = dataB;
	while(!TRANSFER_COMPLETE);

	/* End communications */
	PORTC |= (1 << CS);
}
