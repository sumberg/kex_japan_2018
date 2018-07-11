#include "SPI.h"
#include "expander.h"

void expander_setup(void)
{
	/* Bank 0, sequential operation 0 (default) */
	SPI_write(IOCONA, 0x00);
	/* Set PORTA to output */
	SPI_write(IODIRA, 0x00);
	/* Set PORTB to output */
	SPI_write(IODIRB, 0x00);
}

void expander_set_ports_AB(uint8_t dataA, uint8_t dataB)
{
	SPI_write(GPIOA, dataA);
	SPI_write(GPIOB, dataB);
}
