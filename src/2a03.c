#include "2a03.h"

extern void slave_memory_write12(uint8_t, uint8_t, uint8_t);
extern void slave_memory_write15(uint8_t, uint8_t, uint8_t);
extern void slave_memory_write16(uint8_t, uint8_t, uint8_t);
extern void slave_disable_interrupts12(void);
extern void slave_disable_interrupts15(void);
extern void slave_disable_interrupts16(void);
extern void slave_reset_pc12(void);
extern void slave_reset_pc15(void);
extern void slave_reset_pc16(void);
extern void slave_write_accumulator12(uint8_t);
extern void slave_write_accumulator15(uint8_t);
extern void slave_write_accumulator16(uint8_t);
extern void slave_invert_accumulator12(void);
extern void slave_invert_accumulator15(void);
extern void slave_invert_accumulator16(void);
extern uint8_t slave_fetch_data(void);
extern uint8_t detect(void);
extern void slave_send_instruction_immediate12(uint8_t, uint8_t);
extern void slave_send_instruction_immediate15(uint8_t, uint8_t);
extern void slave_send_instruction_immediate16(uint8_t, uint8_t);
extern void slave_send_instruction_zero_page12(uint8_t, uint8_t);
extern void slave_send_instruction_zero_page15(uint8_t, uint8_t);
extern void slave_send_instruction_zero_page16(uint8_t, uint8_t);
extern void slave_send_instruction_absolute12(uint8_t, uint8_t, uint8_t);
extern void slave_send_instruction_absolute15(uint8_t, uint8_t, uint8_t);
extern void slave_send_instruction_absolute16(uint8_t, uint8_t, uint8_t);

/* The 6502 opcodes needed */
#define LDA_imm 0xA9
#define STA_abs 0x8D
#define STA_zp 0x85
#define JMP_abs 0x4C

/* match io functions with correct timing */
void (*slave_memory_write)(uint8_t, uint8_t, uint8_t);
void (*slave_reset_pc)(void);
void (*slave_disable_interrupts)(void);
void (*slave_write_accumulator)(uint8_t);
void (*slave_invert_accumulator)(void);
void (*slave_send_instruction_immediate)(uint8_t, uint8_t);
void (*slave_send_instruction_zero_page)(uint8_t, uint8_t);
void (*slave_send_instruction_absolute)(uint8_t, uint8_t, uint8_t);

/* Check divider on slave */
uint8_t io_clockdiv = 0;

void slave_write(uint8_t addr_hi, uint8_t addr_lo, uint8_t val)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		slave_memory_write(addr_hi, addr_lo, val);
	}
}

void disable_slave_interrupts(void)
{
	slave_disable_interrupts();
}

void reset_slave_pc(void)
{
	slave_reset_pc();
}

void write_slave_accumulator(uint8_t val)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		slave_write_accumulator(val);
	}
}

uint8_t fetch_slave_data(void)
{
	uint8_t val;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		val = slave_fetch_data();
	}
	return val;
}

void invert_slave_accumulator(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		slave_invert_accumulator();
	}
}

void send_slave_absolute(Instruction *instr)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		slave_send_instruction_absolute(
				instr->opcode,
				instr->operands.abs_addr.hi,
				instr->operands.abs_addr.lo);
	}
}

void send_slave_immediate(Instruction *instr)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		slave_send_instruction_immediate(instr->opcode, instr->operands.imm_val);
	}
}

void send_slave_zero_page(Instruction *instr)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		slave_send_instruction_zero_page(instr->opcode, instr->operands.zp_addr);
	}
}

void send_slave_instruction(Instruction *instr) {
	enum AddrMode am = ROM_getOpcodeAddrMode(instr->opcode);
	switch (am) {
		case ZERO_PAGE:
			send_slave_zero_page(instr);
			break;
		case IMMEDIATE:
			send_slave_immediate(instr);
			break;
		case ABSOLUTE:
			send_slave_absolute(instr);
			break;
		case UNSUPPORTED:
			/* Treat unsupported instructions as idle instructions (STA zp) */
			send_slave_zero_page(instr);
			break;
	}
}

void setup_slave_timing(void)
{
	/* Output STA absolute addressing opcode on data bus to perform detect() */
	PORTD = STA_abs;

	/* Do three tries to detect */
    uint8_t tries = 3;
    while (tries-- > 0) {
        io_clockdiv = detect();
        if (io_clockdiv == 12 || io_clockdiv == 15 || io_clockdiv == 16)
            break;
    }

    /* Point function pointers to correct function based on which 2A03 model is
       being used */
    switch (io_clockdiv) {
	    case 12:
	        slave_memory_write = &slave_memory_write12;
	        slave_reset_pc = &slave_reset_pc12;
	        slave_disable_interrupts = &slave_disable_interrupts12;
			slave_write_accumulator = &slave_write_accumulator12;
			slave_invert_accumulator = &slave_invert_accumulator12;
			slave_send_instruction_immediate = &slave_send_instruction_immediate12;
			slave_send_instruction_zero_page = &slave_send_instruction_zero_page12;
			slave_send_instruction_absolute = &slave_send_instruction_absolute12;
	        break;
	    case 15:
	        slave_memory_write = &slave_memory_write15;
	        slave_reset_pc = &slave_reset_pc15;
	        slave_disable_interrupts = &slave_disable_interrupts15;
			slave_write_accumulator = &slave_write_accumulator15;
			slave_invert_accumulator = &slave_invert_accumulator15;
			slave_send_instruction_immediate = &slave_send_instruction_immediate15;
			slave_send_instruction_zero_page = &slave_send_instruction_zero_page15;
			slave_send_instruction_absolute = &slave_send_instruction_absolute15;
	        break;
	    case 16:
		default:
	        slave_memory_write = &slave_memory_write16;
	        slave_reset_pc = &slave_reset_pc16;
	        slave_disable_interrupts = &slave_disable_interrupts16;
			slave_write_accumulator = &slave_write_accumulator16;
			slave_invert_accumulator = &slave_invert_accumulator16;
			slave_send_instruction_immediate = &slave_send_instruction_immediate16;
			slave_send_instruction_zero_page = &slave_send_instruction_zero_page16;
			slave_send_instruction_absolute = &slave_send_instruction_absolute16;
	        break;
    }

	/* Disable interrupts on RP2A03 */
	disable_slave_interrupts();
	reset_slave_pc();
}
