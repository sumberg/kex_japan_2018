#pragma once

/* SPI.h */
#include <stdint.h>
#include <avr/io.h>

/* Atmega328p SPI master device */
#define SPI_PORT	PORTB
#define SPI_DDR		DDRB
#define SPI_CS		PB2
#define SPI_MOSI	PB3
#define SPI_SCK		PB5

/* MCP23S17 SPI slave device */
#define SPI_SLAVE_ID    	0x40
#define SPI_SLAVE_ADDR  	0x00
#define SPI_SLAVE_WRITE		0x00
#define SPI_WRITE_OPCODE	(SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E) | SPI_SLAVE_WRITE)

// MCP23S17 Registers Definition for BANK=0 (default)
#define IODIRA 0x00
#define IODIRB 0x01
#define IOCONA 0x0A
#define GPIOA  0x12
#define GPIOB  0x13

#define TRANSFER_COMPLETE (SPSR & (1 << SPIF))

/* Set up SPI master-slave relationship */
void SPI_setup();
/* Write to SPI slave */
void SPI_write(uint8_t, uint8_t);
