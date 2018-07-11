/* util/delay.h for debugging */
#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdint.h>
#include "setup.h"
#include "ram.h"
#include "SPI.h"

int main() {

	_delay_ms(500);
	setup();

	uint8_t recv;

	RAM_write(0x40, 0x00, 0x04);
	RAM_write(0x40, 0x01, 0x03);
	RAM_write(0x40, 0x02, 0x02);
	RAM_write(0x40, 0x03, 0x01);
	RAM_write(0x40, 0x04, 0x00);
	_delay_us(1);

	while(1) {
		uint8_t success = 1;

		if ((recv = RAM_read(0x40, 0x00)) != 0x04) success = 0;
		if ((recv = RAM_read(0x40, 0x01)) != 0x03) success = 0;
		if ((recv = RAM_read(0x40, 0x02)) != 0x02) success = 0;
		if ((recv = RAM_read(0x40, 0x03)) != 0x01) success = 0;
		if ((recv = RAM_read(0x40, 0x04)) != 0x00) success = 0;

		PORTB |= (success << 1);
		_delay_ms(500);
		PORTB &= ~(1 << 1);
		_delay_ms(500);
	}

	return 0;
}
