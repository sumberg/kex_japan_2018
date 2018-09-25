/* Main program is used to measure the performance of the implementation by
 * performing a number of tests for different categories of subsets to the
 * 6502 instruction set.
 * */
#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "main.h"
#include "setup.h"
#include "2a03.h"
#include "rom.h"

#define LED_ON() (DEBUG_PORT |= (1 << DEBUG_LED))
#define LED_OFF() (DEBUG_PORT &= ~(1 << DEBUG_LED))
#define CLEAR_STOP() (TIMING_PORT &= ~(1 << STOP_PIN))
#define TIMING_STOP() (TIMING_PORT |= (1 << STOP_PIN))
#define SETUP_DONE() (TIMING_PORT |= (1 << WAIT_PIN))

#define DEFAULTINSTR (uint32_t) 1000
#define MAXINSTR (uint32_t) 5000
#define INCR (uint32_t) 10
#define NUM_CATEGORIES 4

void flash_led(uint8_t);

int globalTimerOverflowTimeout;

enum MODE { DEFAULT, INCR_LENGTH, INCR_RESET, NO_EMUROM_FUNC, DONE };

int main(void)
{
	int done = 0;
	enum MODE mode = DEFAULT;
	/* Starting value when measuring increasing program length */
	uint32_t numInstructions = 10;

	/* Store all categories of ROM to iterate over */
	uint8_t* categories[] = {
		ROM_IMMEDIATE,
		ROM_ZEROPAGE,
		ROM_ABSOLUTE,
		ROM_MIXED
	};

	/* Setup system */
	setup();

	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	instr->opcode = LDA_imm;
	instr->operands.imm_val = 0x42;
	send_slave_instruction(instr);

	/* Tell arduino that setup is done */
	SETUP_DONE();

	/* Go through all of the categories */
	for (int cat = 0; cat < NUM_CATEGORIES; cat++) {
		ROM_setROM(categories[cat]);

		/* Run all tests */
		while (done == 0) {
			/* Wait for start signal */
			while ((TIMING_PORT_IN & (1 << START_PIN)) == 0);
			LED_ON();

			switch (mode) {
				/* Run everything at default values. Measure:
				 * 		- Response time
				 * 		- Data validation
				 * 		- Time of completion WITH ROM functions
				 * */
				case DEFAULT:
					// Validate output and measure response time and
					// time of completion when using ROM functions
					globalTimerOverflowTimeout = TIMER_OVERFLOW_DEFAULT;

					for (uint32_t i = DEFAULTINSTR; i > 0; i--) {
						ROM_nextInstruction(instr);
						send_slave_instruction(instr);
					}
					mode = INCR_LENGTH;
					break;
				/* Sends numInstructions worth of instructions, and increases
				 * numInstructions after each iteration. Measure:
				 * 		- Time of completion as program length is increased
				 * All variables except program length are default values.
				 * */
				case INCR_LENGTH:
					// Measure time of completion as program increases in length
					/* Send instructions */
					for (uint32_t i = numInstructions; i > 0; i--) {
						ROM_nextInstruction(instr);
						send_slave_instruction(instr);
					}
					numInstructions += INCR;
					/* Reset ROM PC and increment number of
					 * instructions for next iteration */
					ROM_resetPC();

					/* Done when MAXINSTR has been tested */
					if (numInstructions > MAXINSTR) {
						mode = INCR_RESET;
						/* Prepare timer overflow values for next test */
						globalTimerOverflowTimeout = TIMER_OVERFLOW_MIN;
					}
					break;
				/* Decrease the frequency of when the reset_slave_pc interrupt
				 * routine is run by increasing the number of timer overflows
				 * needed to trigger. Measure:
				 * 		- Time of completion as reset routine is varied
				 * All variables except reset_slave_pc frequency are default values.
				 * */
				case INCR_RESET:
					for (uint32_t i = DEFAULTINSTR; i > 0; i--) {
						ROM_nextInstruction(instr);
						send_slave_instruction(instr);
					}
					globalTimerOverflowTimeout++;
					if (globalTimerOverflowTimeout >= TIMER_OVERFLOW_MAX)
						mode = NO_EMUROM_FUNC;
					break;
				/* Disables the emulated ROM wrapping functions, and sends instructions
				 * manually from the respecitve ROMs. Measure:
				 * 		- Time of completion WITHOUT ROM functions
				 * All variables except ROM functionality are default values.
				 * */
				case NO_EMUROM_FUNC:
					// Measure time of completion without emulated ROM functions
					mode = DONE;
					break;
					/* When all tests are done, break the loop */
				case DONE:
					done = 1;
					break;
			}

			/* Send stop signal for timing */
			TIMING_STOP();
			CLEAR_STOP();

			LED_OFF();
		}

	}

	/* Idle when all tests are done */
	while (1);

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
