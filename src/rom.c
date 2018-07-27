#include "rom.h"

/* A Program is a set of instructions contained within ROM, and
 * is run in the range of its start_instruction to end_instruction. */
typedef struct {
	int start_instruction;
	int end_instruction;
} Program;

/* ROM "program counter" */
static uint16_t nxt = 0;

/* Gets the next byte from ROM */
uint8_t ROM_fetchNextByte(uint8_t *prgROM)
{
	return prgROM[nxt++];
}

/* Returns 'true' if ROM still contains instructions, 'false' otherwise */
uint8_t ROM_instructionsLeft(uint8_t *prgROM)
{
	/* If three contiguous bytes contain 0xFF, we have reached end of program */
	return (prgROM[nxt] != 0xFF) && (prgROM[nxt + 1] != 0xFF) && (prgROM[nxt + 2] != 0xFF);
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
void ROM_nextInstruction(uint8_t *prgROM, Instruction *instr)
{
	if (!ROM_instructionsLeft(prgROM))
		ROM_resetPC();

	instr->opcode = ROM_fetchNextByte(prgROM);
	enum AddrMode am = ROM_getOpcodeAddrMode(instr->opcode);

	switch(am) {
		case ZERO_PAGE:
			instr->operands.zp_addr = ROM_fetchNextByte(prgROM);
			break;
		case IMMEDIATE:
			instr->operands.imm_val = ROM_fetchNextByte(prgROM);
			break;
		case ABSOLUTE:
			instr->operands.abs_addr.lo = ROM_fetchNextByte(prgROM);
			instr->operands.abs_addr.hi = ROM_fetchNextByte(prgROM);
			break;
		case UNSUPPORTED:
			/* Treat unsupported instructions as idle mode (STA zp) */
			instr->opcode = 0x85;
			instr->operands.zp_addr = 0x85;
	}
}

/* Write numBytes worth of bytes from ROM to RAM chip, from starting address 0x8000,
 * and verify that write is successful.
 * NOTE: Assumes that ROM <= available RAM memory
 * Params:
 * 		numBytes - number of program ROM bytes to copy
 * Return:
 * 		1 if success, 0 otherwise
 */
uint8_t ROM_TO_RAM(uint8_t *prgROM)
{
	ROM_resetPC();

	uint16_t addr = 0x8000;
	uint8_t nxtByte, addr_hi, addr_lo;
	addr_hi = addr_lo = 0;
	while (ROM_instructionsLeft(prgROM)) {
		PORTB |= (1 << PB1);
		nxtByte = ROM_fetchNextByte(prgROM);
		addr_hi = ((addr & ADDR_HI_MASK) >> 8);
		addr_lo = (addr & ADDR_LO_MASK);
		RAM_write(addr_hi, addr_lo, nxtByte);
		addr++;
	}

	/* Write end loop at address 0xFFF0 */
	RAM_write(0xFF, 0xF0, JMP_abs);
	RAM_write(0xFF, 0xF1, 0xF0);
	RAM_write(0xFF, 0xF2, 0xFF);

	/* Set reset vector (0xFFFC-0xFFFD) to point to start of program memory
	* (0x8000) */
	RAM_write(0xFF, 0xFC, 0x00);
	RAM_write(0xFF, 0xFD, 0x80);

	/* Verify write success */
	addr = 0x8000;
	ROM_resetPC();
	uint8_t tmp;
	uint8_t success = 1;

	/* Verify program ROM */
	while (ROM_instructionsLeft(prgROM)) {
		nxtByte = ROM_fetchNextByte(prgROM);
		addr_hi = ((addr & ADDR_HI_MASK) >> 8);
		addr_lo = (addr & ADDR_LO_MASK);
		tmp = RAM_read(addr_hi, addr_lo);
		/* fuck this rad */
		if (tmp != nxtByte) success = 0;
		addr++;
	}

	/* Verify end loop */
	if (RAM_read(0xFF,0xF0) != JMP_abs) success = 0;
	if (RAM_read(0xFF,0xF1) != 0xF0) success = 0;
	if (RAM_read(0xFF,0xF2) != 0xFF) success = 0;
	/* Verify reset vector */
	if (RAM_read(0xFF, 0xFC) != 0x00) success = 0;
	if (RAM_read(0xFF, 0xFD) != 0x80) success = 0;

	return success;
}
