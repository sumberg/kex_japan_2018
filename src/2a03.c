#include "2a03.h"

extern void slave_memory_write(uint8_t, uint8_t, uint8_t);
extern void slave_disable_interrupts(void);

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
