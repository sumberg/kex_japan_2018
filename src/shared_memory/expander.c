#include "expander.h"

void setup_expander(void)
{
	/* Activate communication (CS->LOW) */
	PORTC &= ~(1 << CS);

	/* Send opcode */
	SPDR = OPCODE_WRITE;
	while(!TRANSFER_COMPLETE);

	/* Set PORTA and PORTB as input */
	SPDR = EXPANDER_DIRA;
	while(!TRANSFER_COMPLETE);
	SPDR = 0x00;
	while(!TRANSFER_COMPLETE);
	SPDR = 0x00;
	while(!TRANSFER_COMPLETE);

	/* Deactivate communication (CS->HIGH) */
	PORTC |= (1 << CS);
}

/* Write data to expander address addr */
void expander_write(uint8_t addr, uint8_t data)
{
	/* Activate communication, and send opcode */
	PORTC &= ~(1 << CS);
	SPDR = OPCODE_WRITE;
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
	SPDR = OPCODE_WRITE;
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
