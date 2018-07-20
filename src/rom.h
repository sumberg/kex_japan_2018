#pragma once

#include <stdint.h>

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
	JMP_abs = 0x4C,
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


/* Union of structs divided into different addressing modes */
typedef struct {
	uint8_t opcode;
	union {
		uint8_t zp_addr;
		uint8_t imm_val;
		struct {
			uint8_t hi;
			uint8_t lo;
		} abs_addr;
	} operands;
} Instruction;

enum AddrMode { ZERO_PAGE, IMMEDIATE, ABSOLUTE, UNSUPPORTED };

/* Return next byte of ROM */
uint8_t ROM_fetchNextByte(void);
/* Returns 'true' if ROM still contains instructions, 'false' otherwise */
uint8_t ROM_instructionsLeft(void);
/* Write entire ROM to RAM chip, from starting address 0x8000 (mask 0x7FFF)
 * and verifies */
uint8_t ROM_TO_RAM(void);
/* Fetches the next instruction from ROM, and places it into the supplied
 * instruction struct */
void ROM_nextInstruction(Instruction*);
/* Gets the addressing mode for the instruction based on its opcode */
enum AddrMode ROM_getOpcodeAddrMode(uint8_t opcode);
/* Reset ROM program counter to beginning of ROM */
void ROM_resetPC(void);
