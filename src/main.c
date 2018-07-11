/* util/delay.h for debugging */
#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdint.h>
#include "setup.h"
#include "ram.h"
#include "SPI.h"
#include "rom.h"

int main() {
	_delay_ms(500); // TODO remove this?
	setup();

	uint8_t nxtByte;
	uint16_t addr = 0x8000;
	uint8_t addr_hi, addr_lo;

	/* Write entire ROM to RAM chip, from starting address 0x8000 (mask 0x7FFF)*/
	while (ROM_instructionsLeft()) {
		nxtByte = ROM_fetchNextByte();
		addr_hi = ((addr & 0xFF00) >> 8);
		addr_lo = (addr & 0x00FF);
		RAM_write(addr_hi, addr_lo, nxtByte);
		addr++;
	}

	/* Verify RAM write */
	addr = 0x8000;
	ROM_resetPC();
	uint8_t tmp;
	uint8_t success = 1;
	while (ROM_instructionsLeft()) {
		nxtByte = ROM_fetchNextByte();
		addr_hi = ((addr & 0xFF00) >> 8);
		addr_lo = (addr & 0x00FF);
		tmp = RAM_read(addr_hi, addr_lo);
		addr+++;
		if (tmp != nxtByte)
			success = 0;
	}

	PORTB |= (success << DEBUG_LED);

	while(1) {}

	return 0;
}
