#include <stdint.h>

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

/* Returns 'true' if ROM still contains instructions, 'false' otherwise */
uint8_t instructionsLeft(void);
/* Fetches the next instruction from ROM, and places it into the supplied
 * instruction struct */
void ROM_nextInstruction(Instruction*);
/* Gets the addressing mode for the instruction based on its opcode */
enum AddrMode getOpcodeAddrMode(uint8_t opcode);
/* Reset ROM program counter to beginning of ROM */
void resetROMPC(void);
