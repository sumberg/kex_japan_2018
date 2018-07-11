#include "rom.h"

enum OPCODES {
	ADC_abs = 0x6D,
	ADC_imm = 0x69,
	ADC_zp = 0x65,
	ASL_acc = 0x0A, // TODO this is unsupported
	ASL_zp = 0x06,
	ASL_abs = 0x0E,
	EOR_abs = 0x4D,
	EOR_imm = 0x49,
	EOR_zp = 0x45,
	LDA_abs = 0xAD,
	LDA_imm = 0xA9,
	LDA_zp = 0xA5,
	ORA_abs = 0x0D,
	ORA_imm = 0x09,
	ORA_zp = 0x05,
	SBC_abs = 0xED,
	SBC_imm = 0xE9,
	SBC_zp = 0xE5,
	STA_abs = 0x8D,
	STA_zp = 0x85,
};

/* A Program is a set of instructions contained within ROM, and
 * is run in the range of its start_instruction to end_instruction. */
typedef struct {
	int start_instruction;
	int end_instruction;
} Program;

/* Program ROM here */
static uint8_t ROM[512] = {
	/* Test simple store and load */
	LDA_imm, 0xAA,
	STA_zp, 0x20, // zero page 0x20 should now contain 0xAA
	LDA_imm, 0x55,
	STA_zp, 0x21, // zero page 0x21 should now contain 0x55
	LDA_imm, 0x66,
	STA_zp, 0x22, // zero page 0x22 should now contain 0x66

	/* Test logical operations */
	LDA_zp, 0x20,
	ORA_zp, 0x21,
	STA_zp, 0x23, // zero page 0x23 should now contain 0xFF
	LDA_zp, 0x22,
	EOR_imm, 0xFF,
	STA_zp, 0x24, // zero page 0x24 should now contain 0x99
	LDA_zp, 0x23,
	ORA_zp, 0x24,
	ORA_imm, 0x24,
	STA_zp, 0x25, // zero page 0x25 should now contain 0xBD
	STA_zp, 0x26,
	ASL_zp, 0x26, // zero page 0x26 should now contain 0x7A
	LDA_zp, 0x26,
	STA_zp, 0x27,
	ASL_zp, 0x27, // zero page 0x27 should now contain 0xF4

	/* Test arithmetic operations */
	LDA_imm, 0x00,
	ADC_imm, 0x01,
	STA_zp, 0x28, // zero page 0x28 should now contain 0x01
	ADC_zp, 0x28,
	STA_zp, 0x29, // zero page 0x29 should now contain 0x02
	ADC_zp, 0x29,
	STA_zp, 0x2A, // zero page 0x2A should now contain 0x04
	ADC_zp, 0x2A,
	STA_zp, 0x2B, // zero page 0x2B should now contain 0x08
	ADC_zp, 0x2B,
	STA_zp, 0x2C, // zero page 0x2C should now contain 0x10
	STA_zp, 0x2D,
	SBC_imm, 0x01, // zero page 0x2D should now contain 0x0F
	LDA_zp, 0x2D,
	STA_zp, 0x2E,
	SBC_imm, 0x01,
	SBC_imm, 0x01,
	SBC_imm, 0x01,
	SBC_imm, 0x01,
	SBC_imm, 0x01, // zero page 0x2E should now contain 0x0A

	/* Test APU store and loads */
	LDA_imm, 0x30,
	STA_abs, 0x00, 0x40, // 0x4000 should now contain 0x30
	LDA_imm, 0x00,
	STA_abs, 0x01, 0x40,
	STA_abs, 0x02, 0x40,
	STA_abs, 0x03, 0x40, // 0x4001-0x4003 should now contain 0x00
	LDA_abs, 0x00, 0x40,
	STA_zp, 0x2F, // zero page 0x2F should now (maybe) contain 0x30

	/* End of instructions */
	0xFF, 0xFF
};

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

/* Write entire ROM to RAM chip, from starting address 0x8000 (mask 0x7FFF),
 * and verifies that write is successful.
 * Return: success - 1 if success, 0 otherwise
 */
uint8_t ROM_TO_RAM(void)
{
	uint16_t addr = 0x8000;
	uint8_t nxtByte, addr_hi, addr_lo;
	addr_hi = addr_lo = 0;
	while (ROM_instructionsLeft()) {
		nxtByte = ROM_fetchNextByte();
		addr_hi = ((addr & 0xFF00) >> 8);
		addr_lo = (addr & 0x00FF);
		RAM_write(addr_hi, addr_lo, nxtByte);
		addr++;
	}

	/* Write end loop at address 0xFF00 */
	RAM_write(0xFF, 0x00, JMP_abs);
	RAM_write(0xFF, 0x01, 0x00);
	RAM_write(0xFF, 0x02, 0xFF);

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
	while (ROM_instructionsLeft()) {
		nxtByte = ROM_fetchNextByte();
		addr_hi = ((addr & 0xFF00) >> 8);
		addr_lo = (addr & 0x00FF);
		tmp = RAM_read(addr_hi, addr_lo);
		addr++;
		if (tmp != nxtByte)
			success = 0;
	}
	/* Verify reset vector */
	if ((tmp = RAM_read(0xFF, 0xFC)) != 0x00) success = 0;
	if ((tmp = RAM_read(0xFF, 0xFD)) != 0x80) success = 0;

	return success;
}
