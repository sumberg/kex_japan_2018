#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "setup.h"
#include "2a03.h"
#include "rom.h"

#define LED_ON() (DEBUG_PORT |= (1 << DEBUG_LED))
#define LED_OFF() (DEBUG_PORT &= ~(1 << DEBUG_LED))

#define INCR (uint32_t) 10

void flash_led(uint8_t);

int main(void)
{
	/* Setup system */
	setup();

	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	/* Perform all statically programmed instruction */
	while (ROM_instructionsLeft()) {
		ROM_nextInstruction(instr);
		send_slave_instruction(instr);
	}

	/* Idle indefinitely */
	while (1) {
	}

	return 0;
}

void flash_led(uint8_t times)
{
	for (; times > 0; times--)
	{
		LED_ON();
		_delay_ms(500);
		LED_OFF();
		_delay_ms(500);
	}
	_delay_ms(1000);
}
