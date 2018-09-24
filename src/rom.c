#include "rom.h"

/* A Program is a set of instructions contained within ROM, and
 * is run in the range of its start_instruction to end_instruction. */
typedef struct {
	int start_instruction;
	int end_instruction;
} Program;

static uint8_t *ROM;

/* ROM "program counter" */
static uint16_t nxt = 0;

/* Gets the next byte from ROM */
uint8_t ROM_fetchNextByte(void)
{
	return ROM[nxt++];
}

/* Returns 'true' if ROM still contains instructions, 'false' otherwise */
uint8_t ROM_instructionsLeft(void)
{
	return ROM[nxt] != 0xFF;
}

/* Gets the addressing mode of operation */
enum AddrMode ROM_getOpcodeAddrMode(uint8_t opcode)
{
	uint8_t offset = opcode & 0x0F;
	if (offset >= 0x04 && offset <= 0x07)
		return ZERO_PAGE;
	else if (offset == 0x09 || offset == 0x0B)
		return IMMEDIATE;
	else if (offset >= 0x0C && offset <= 0x0F)
		return ABSOLUTE;
	else
		return UNSUPPORTED;
}

/* Resets the "program counter" for ROM to 0. */
void ROM_resetPC(void)
{
	nxt = 0;
}

/* Fetches the next instruction from ROM, and places it into the supplied
 * instruction struct. If memory has reached end of instructions, wrap around
 * and start fetching instructions from the beginning. */
void ROM_nextInstruction(Instruction *instr)
{
	if (!ROM_instructionsLeft())
		ROM_resetPC();

	instr->opcode = ROM_fetchNextByte();
	enum AddrMode am = ROM_getOpcodeAddrMode(instr->opcode);

	switch(am) {
		case ZERO_PAGE:
			instr->operands.zp_addr = ROM_fetchNextByte();
			break;
		case IMMEDIATE:
			instr->operands.imm_val = ROM_fetchNextByte();
			break;
		case ABSOLUTE:
			instr->operands.abs_addr.lo = ROM_fetchNextByte();
			instr->operands.abs_addr.hi = ROM_fetchNextByte();
			break;
		case UNSUPPORTED:
			/* Treat unsupported instructions as idle mode (STA zp) */
			instr->opcode = 0x85;
			instr->operands.zp_addr = 0x85;
	}
}

void ROM_setROM(uint8_t *newROM)
{
	ROM = newROM;
}
