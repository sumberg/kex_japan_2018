#include "rom.h"

/* Program ROM here */
static uint8_t ROM[256] = {
	0xA9, 0x01,
	0x49, 0xFF,
	0xA9, 0x02,
	0x49, 0xFF,
	0xA9, 0x03,
	0x49, 0xFF,
	0xA9, 0x04,
	0x49, 0xFF,
	0xA9, 0x05,
	0x49, 0xFF,
	0xA9, 0x06,
	0x49, 0xFF,
	0xA9, 0x01,
	0x49, 0xFF,
};

static uint8_t nxt = 0;
static int memoryLeft = 256;

/* Gets the next byte from ROM */
static uint8_t ROM_fetchNextByte(void)
{
	if ((--memoryLeft) < 0)
		memoryLeft = 0;
	return ROM[nxt++];
}

/* Returns 'true' if ROM still contains instructions, 'false' otherwise */
uint8_t instructionsLeft(void)
{
	return memoryLeft > 0;
}

/* Gets the addressing mode of operation */
enum AddrMode getOpcodeAddrMode(uint8_t opcode)
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

/* Fetches the next instruction from ROM, and places it into the supplied
 * instruction struct */
void ROM_nextInstruction(Instruction *instr)
{
	instr->opcode = ROM_fetchNextByte();
	enum AddrMode am = getOpcodeAddrMode(instr->opcode);

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
