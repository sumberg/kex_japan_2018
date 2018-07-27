/* util/delay.h for debugging */
#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdint.h>
#include "setup.h"
#include "ram.h"
#include "SPI.h"
#include "rom.h"

#define LED_ON() (DEBUG_PORT |= (1 << DEBUG_LED))
#define LED_OFF() (DEBUG_PORT &= ~(1 << DEBUG_LED))

#define SIGNAL_READY() (TIMING_PORT |= (1 << READY_PIN))
#define SIGNAL_STOP() (TIMING_PORT |= (1 << STOP_PIN))
#define SIGNAL_START (TIMING_PORT_IN & (1 << START_PIN))

void start_slave(void);
void stop_slave(void);
void flash_led(uint8_t);

int main()
{
	uint8_t success = 0;
	uint8_t ramVal;
	uint16_t addresses[10] = {0};


	setup();

	/* Indicate if write was a success */
	if (ROM_TO_RAM()) LED_ON();
	_delay_ms(1000);
	LED_OFF();
	/* Wait 1 sec before starting slave, to ensure stability */
	_delay_ms(1000);

	start_slave();

	/* Stop slave when it has reached endless loop address (0xFF00) */
	uint16_t stop;
	while(1) {
		stop = SPI_read(GPIOA) << 8;
		stop |= SPI_read(GPIOB);
		stop |= 0x8000;
		if (stop == 0xFFFC || stop == 0xFFFD) LED_ON();
		if (stop >= 0xFF00 && stop <= 0xFF02)  {
			stop_slave();
			break;
		}
	}

	/*success = 1;*/
	/*if (RAM_read(0x85, 0x00) != 0xAA) success = 0;*/
	/*if (RAM_read(0x85, 0x01) != 0x55) success = 0;*/
	/*if (RAM_read(0x85, 0x02) != 0x66) success = 0;*/
	/*if (RAM_read(0x85, 0x03) != 0xFF) success = 0;*/

	// DEBUG_PORT |= (success << DEBUG_LED);

	/* If above tests are a failure, test what values
	 * are actually written to the addresses like this */
	uint8_t offset = 0x00;
	ramVal = RAM_read(0x60, offset);
	while(1) {
		flash_led(ramVal / 16);
		flash_led(ramVal % 16);
		_delay_ms(1000);
		offset = (offset + 1) % 4;
	}

	return 0;
}

/* Sets necessary ports and starts slave program */
void start_slave(void)
{
	DATA_DDR = 0x00;

	SPI_write(IODIRA, 0xFF);
	SPI_write(IODIRB, 0xFF);

	RAM_CTRL_DDR &= ~((1 << RAM_RW) | (1 << RAM_OE));
	RAM_CTRL_PORT &= ~((1 << RAM_RW) | (1 << RAM_CE) | (1 << RAM_OE));
	CTRL_PORT |= (1 << CTRL_RESET_SLAVE);
}

/* Stops slave execution, and gives control of all components to master */
void stop_slave(void)
{
	RAM_CTRL_DDR |= (1 << RAM_RW) | (1 << RAM_CE) | (1 << RAM_OE);
	RAM_CTRL_PORT |= (1 << RAM_RW) | (1 << RAM_CE);
	CTRL_PORT &= ~(1 << CTRL_RESET_SLAVE);

	RAM_CTRL_PORT &= ~(1 << RAM_OE);

	DATA_DDR = 0xFF;
	SPI_write(IODIRA, 0x00);
	SPI_write(IODIRB, 0x00);
}

/* Flash debug led val number of times */
void flash_led(uint8_t val)
{
	for (uint8_t i = val; i > 0; i--) {
		LED_ON();
		_delay_ms(500);
		LED_OFF();
		_delay_ms(500);
	}
	_delay_ms(1000);
}
