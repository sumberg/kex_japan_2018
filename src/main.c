#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "setup.h"
#include "2a03.h"
#include "rom.h"

#define LED_ON() (DEBUG_PORT |= (1 << DEBUG_LED))
#define LED_OFF() (DEBUG_PORT &= ~(1 << DEBUG_LED))
#define CLEAR_STOP() (TIMING_PORT &= ~(1 << STOP_PIN))
#define TIMING_STOP() (TIMING_PORT |= (1 << STOP_PIN))

#define INCR (uint32_t) 10

void flash_led(uint8_t);

int main(void)
{
	uint32_t numInstructions = 10;

	/* Setup system */
	setup();

	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	instr->opcode = LDA_imm;
	instr->operands.imm_val = 0x42;
	send_slave_instruction(instr);

	TIMING_PORT |= (1 << PB4);

	/* Idle indefinitely */
	while (1) {
		/* Wait for start signal */
		while ((TIMING_PORT_IN & (1 << START_PIN)) == 0);
		LED_ON();

		/* Send instructions */
		for (uint32_t i = numInstructions; i > 0; i--) {
			ROM_nextInstruction(instr);
			send_slave_instruction(instr);
		}

		/* Send stop signal for timing */
		TIMING_STOP();
		CLEAR_STOP();

		/* Reset ROM PC and increment number of instructions for next iteration */
		ROM_resetPC();
		numInstructions += INCR;
		LED_OFF();
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
