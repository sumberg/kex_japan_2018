#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "setup.h"
#include "2a03.h"

int main(void)
{
	/* Setup system */
	setup();

	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	/* Perform all statically programmed instruction */
	while (instructionsLeft()) {
		ROM_nextInstruction(instr);
		send_slave_instruction(instr);
	}

	/* Idle indefinitely */
	while (1) {
	}

	return 0;
}
