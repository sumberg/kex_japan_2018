/* util/delay.h for debugging */
#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdint.h>
#include "setup.h"
#include "ram.h"
#include "SPI.h"
#include "rom.h"

/* Sets necessary ports and starts slave program */
void start_slave(void);
/* Stops slave execution, and gives control of all components to master */
void stop_slave(void);

int main()
{
	setup();

	SPI_write(IODIRA, 0xFF);
	ROM_TO_RAM();

	start_slave();

	uint16_t stop;
	uint8_t times;
	while(1) {
		stop = SPI_read(GPIOA) << 8;
		stop |= SPI_read(GPIOB);
		stop |= 0x8000;
		if (stop >= 0xFF00 && stop <= 0xFF02) break;
	}

	stop_slave();

	// uint8_t success = 1;
	// if (RAM_read(0x85, 0x00) != 0xAA) success = 0;
	// if (RAM_read(0x85, 0x01) != 0x55) success = 0;
	// if (RAM_read(0x85, 0x02) != 0x66) success = 0;
	// if (RAM_read(0x85, 0x03) != 0xFF) success = 0;

	// DEBUG_PORT |= (success << DEBUG_LED);
	uint8_t read = RAM_read(0x85, 0x00);
	read /= 10;
	while(1) {
		for (int i = read; i > 0; i--) {
			DEBUG_PORT |= (1 << DEBUG_LED);
			_delay_ms(500);
			DEBUG_PORT &= ~(1 << DEBUG_LED);
			_delay_ms(500);
		}
		_delay_ms(1000);
	}

	return 0;
}

void start_slave(void)
{
	DATA_DDR = 0x00;
	SPI_write(IODIRA, 0xFF);
	SPI_write(IODIRB, 0xFF);
	RAM_CTRL_PORT &= ~((1 << RAM_RW) | (1 << RAM_CE));

	CTRL_PORT |= (1 << CTRL_RESET_SLAVE);
}

void stop_slave(void)
{
	DATA_DDR = 0xFF;
	SPI_write(IODIRA, 0x00);
	SPI_write(IODIRB, 0x00);
	RAM_CTRL_PORT |= (1 << RAM_RW) | (1 << RAM_CE);

	CTRL_PORT &= ~(1 << CTRL_RESET_SLAVE);
}
