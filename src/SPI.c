#include "SPI.h"

void SPI_write(uint8_t addr, uint8_t data)
{
	/* Activate CS pin */
	SPI_PORT &= ~(1 << SPI_CS);

	/* Start SPI slave transmission */
	SPDR = SPI_WRITE_OPCODE;

	/* Wait for transmission to complete */
	while (!TRANSFER_COMPLETE);

	/* Start slave register address transmission */
	SPDR = addr;

	/* Wait for transmission to complete */
	while (!TRANSFER_COMPLETE);

	/* Start slave data transmission */
	SPDR = data;

	/* Wait for transmission to complete */
	while (!TRANSFER_COMPLETE);

	/* Deactivate CS pin */
	SPI_PORT |= (1 << SPI_CS);
}

/* SPI setup */
void SPI_setup()
{
	/* Set SPI pins as output */
	SPI_DDR = (1 << SPI_CS) | (1 << SPI_MOSI) | (1 << SPI_SCK);
	/* Set CS */
	SPI_PORT |= (1 << SPI_CS);
}
