#include <stdint.h>
#include "ram.h"

#define ROM_BASEADDR 8000
#define ROM_ENDADDR 0xFFED
#define ROM_MAXSIZE_BYTES (ROM_ENDADDR - ROM_BASEADDR)

#define ADDR_HI_MASK 0xFF00
#define ADDR_LO_MASK 0x00FF

enum OPCODES {
	ADC_abs = 0x6D,
	ADC_imm = 0x69,
	ADC_zp = 0x65,
	ASL_acc = 0x0A, // TODO this is unsupported
	ASL_zp = 0x06,
	ASL_abs = 0x0E,
	CLD = 0xD8,
	EOR_abs = 0x4D,
	EOR_imm = 0x49,
	EOR_zp = 0x45,
	INX = 0xE8,
	JMP_abs = 0x4C,
	LDA_abs = 0xAD,
	LDA_imm = 0xA9,
	LDA_zp = 0xA5,
	LDX_abs = 0xAE,
	LDX_imm = 0xA2,
	LDX_zp = 0xA6,
	ORA_abs = 0x0D,
	ORA_imm = 0x09,
	ORA_zp = 0x05,
	SBC_abs = 0xED,
	SBC_imm = 0xE9,
	SBC_zp = 0xE5,
	SEI = 0x78,
	STA_abs = 0x8D,
	STA_zp = 0x85,
	STX_abs = 0x8E,
	STX_zp = 0x86,
	TXS = 0x9A,
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
uint8_t ROM_fetchNextByte(uint8_t *prgROM);
/* Returns 'true' if ROM still contains instructions, 'false' otherwise */
uint8_t ROM_instructionsLeft(uint8_t *prgROM);
/* Write numBytes worth of ROM to RAM chip, from starting address 0x8000 (mask 0x7FFF)
 * and verifies */
uint8_t ROM_TO_RAM(uint8_t *prgROM);
/* Fetches the next instruction from ROM, and places it into the supplied
 * instruction struct */
void ROM_nextInstruction(uint8_t *prgROM, Instruction*);
/* Gets the addressing mode for the instruction based on its opcode */
enum AddrMode ROM_getOpcodeAddrMode(uint8_t opcode);
/* Reset ROM program counter to beginning of ROM */
void ROM_resetPC(void);
