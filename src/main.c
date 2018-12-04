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
#define SLAVE_WAITING() (DEBUG_PORT |= (1 << DONE_LED))
#define CLEAR_SLAVE_WAITING() (DEBUG_PORT &= ~(1 << DONE_LED))
#define CLEAR_STOP() (TIMING_PORT &= ~(1 << STOP_SLAVE_EXEC))
#define TIMING_STOP() (TIMING_PORT |= (1 << STOP_SLAVE_EXEC))
#define SETUP_DONE() (TIMING_PORT |= (1 << SETUP_DONE_PIN))
#define ARDUINO_READY() (TIMING_PORT_IN & (1 << START_SLAVE_EXEC))

#define INSTRS_DEFAULT (uint32_t) 1000
#define INSTRS_MAX (uint32_t) 5000
#define INSTR_INCR (uint32_t) 10

void flash_led(uint8_t);

int globalTimerOverflowTimeout = TIMER_OVERFLOW_DEFAULT;

enum MODE { DEFAULT, INCR_LENGTH, INCR_RESET, NO_EMUROM_FUNC, DONE };

int main(void)
{
	/* ROM to be used in test */
	/* ROM_setROM(ROM_MIXED); */
	/* Set category to test without ROM emulation */
	enum AddrMode addrMode = ABSOLUTE;

	/* Setup system */
	setup();

	/* Run default test */
	/* testDefaultSettings(); */

	/* Run Increasing Instructions test */
	/* testIncreasingInstructions(); */

	/* Run Increasing Reset Timeout test */
	/* testIncreasingResetTimeout(); */

	/* Run No ROM Emulation Functions test */
	testNoROMEmulationFunctions(addrMode);

	/* Idle when all tests are done */
	LED_ON();
	while (1) {}

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

void testIncreasingInstructions()
{
	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	/* Tell arduino that setup is done */
	LED_ON();
	CLEAR_SLAVE_WAITING();
	CLEAR_STOP();
	SETUP_DONE();

	/* Increment Instructions test; */
	/* Run tests from INSTR_INCR to INSTRS_DEFAULT number of instructions */
	uint32_t numInstructionsSent = 0;
	for (uint32_t i = INSTR_INCR; i <= INSTRS_MAX; i += INSTR_INCR) {
		CLEAR_STOP();
		SLAVE_WAITING();
		while (!ARDUINO_READY());
		CLEAR_SLAVE_WAITING();
		LED_OFF();
		numInstructionsSent = 0;
		while (numInstructionsSent < i) {
			/* Get next instruction */
			ROM_nextInstruction(instr);
			/* Send instruction to slave */
			send_slave_instruction(instr);
			numInstructionsSent++;
		}
		TIMING_STOP();
		_delay_ms(10);
	}

	return;
}

void testDefaultSettings()
{
	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	/* Tell arduino that setup is done */
	LED_ON();
	CLEAR_SLAVE_WAITING();
	CLEAR_STOP();
	SETUP_DONE();

	uint32_t numInstructionsSent = 0;
	/* Run 50 iterations to get a good average */
	for (int i = 0; i < 50; ++i) {
		CLEAR_STOP();
		SLAVE_WAITING();
		while (!ARDUINO_READY());
		CLEAR_SLAVE_WAITING();
		LED_OFF();
		numInstructionsSent = 0;
		while (numInstructionsSent < INSTRS_DEFAULT) {
			/* Get next instruction */
			ROM_nextInstruction(instr);
			/* Send instruction to slave */
			send_slave_instruction(instr);
			numInstructionsSent++;
		}
		TIMING_STOP();
		_delay_ms(10);
	}

	return;
}

void testIncreasingResetTimeout()
{
	/* Instruction struct used for sending instructions */
	Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

	/* Tell arduino that setup is done */
	LED_ON();
	CLEAR_SLAVE_WAITING();
	CLEAR_STOP();
	SETUP_DONE();

	uint32_t numInstructionsSent = 0;
	/* Loop over all reset timing settings, send default num instructions  */
	for (int i = TIMER_OVERFLOW_MAX; i >= TIMER_OVERFLOW_MIN; i--) {
		globalTimerOverflowTimeout = i;
		CLEAR_STOP();
		SLAVE_WAITING();
		while (!ARDUINO_READY());
		CLEAR_SLAVE_WAITING();
		LED_OFF();
		numInstructionsSent = 0;
		while (numInstructionsSent < INSTRS_MAX) {
			/* Get next instruction */
			ROM_nextInstruction(instr);
			/* Send instruction to slave */
			send_slave_instruction(instr);
			numInstructionsSent++;
		}
		TIMING_STOP();
		_delay_ms(10);
	}

	return;
}

void testNoROMEmulationFunctions(enum AddrMode addrMode)
{
	/* Instruction struct used for sending instructions */
	Instruction *imm_instr = (Instruction *) malloc(sizeof(Instruction));
	Instruction *zp_instr = (Instruction *) malloc(sizeof(Instruction));
	Instruction *abs_instr = (Instruction *) malloc(sizeof(Instruction));

	imm_instr->opcode = LDA_imm;
	imm_instr->operands.imm_val = 0x42;

	zp_instr->opcode = LDA_zp;
	zp_instr->operands.zp_addr = 0x20;

	abs_instr->opcode = LDA_abs;
	abs_instr->operands.abs_addr.hi = 0x40;
	abs_instr->operands.abs_addr.lo = 0x00;

	/* Tell arduino that setup is done */
	LED_ON();
	CLEAR_SLAVE_WAITING();
	CLEAR_STOP();
	SETUP_DONE();

	uint32_t numInstructionsSent = 0;
	/* Run 50 iterations to get a good average */
	for (int i = 0; i < 50; ++i) {
		CLEAR_STOP();
		SLAVE_WAITING();
		while (!ARDUINO_READY());
		CLEAR_SLAVE_WAITING();
		LED_OFF();
		numInstructionsSent = 0;
		while (numInstructionsSent < INSTRS_DEFAULT) {
			switch (addrMode) {
				case IMMEDIATE:
					send_slave_instruction(imm_instr);
					break;
				case ZERO_PAGE:
					send_slave_instruction(zp_instr);
					break;
				case ABSOLUTE:
					send_slave_instruction(abs_instr);
					break;
			}
		numInstructionsSent++;
		}
		TIMING_STOP();
		_delay_ms(10);
	}

	return;
}
