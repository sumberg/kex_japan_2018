#include "2a03.h"

extern void slave_memory_write12(uint8_t, uint8_t, uint8_t);
extern void slave_memory_write15(uint8_t, uint8_t, uint8_t);
extern void slave_memory_write16(uint8_t, uint8_t, uint8_t);
extern void slave_disable_interrupts12(void);
extern void slave_disable_interrupts15(void);
extern void slave_disable_interrupts16(void);
extern uint8_t detect(void);

/* The 6502 opcodes needed */
#define LDA_imm 0xA9
#define STA_abs 0x8D
#define STA_zp 0x85
#define JMP_abs 0x4C

/* match io functions with correct timing */
void (*slave_memory_write)(uint8_t, uint8_t, uint8_t);
void (*slave_disable_interrupts)(void);

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

void setup_slave_timing(void)
{
	/* Debug LEDs for detecting slave divider */
	uint8_t offset = 2;
	DDRC |= (0xF << offset);

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
	        slave_memory_write= &slave_memory_write12;
	        slave_disable_interrupts = &slave_disable_interrupts12;
			PORTC = (1 << offset);
	        break;
	    case 15:
	        slave_memory_write = &slave_memory_write15;
	        slave_disable_interrupts = &slave_disable_interrupts15;
			PORTC = (2 << offset);
	        break;
	    case 16:
	        slave_memory_write = &slave_memory_write16;
	        slave_disable_interrupts = &slave_disable_interrupts16;
			PORTC = (4 << offset);
	        break;
		default:
	        slave_memory_write = &slave_memory_write16;
	        slave_disable_interrupts = &slave_disable_interrupts16;
			PORTC = (8 << offset);
	        break;
    }

	/* Disable interrupts on RP2A03 */
	disable_slave_interrupts();
}
