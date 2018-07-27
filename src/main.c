/* util/delay.h for debugging */
#define F_CPU 20000000UL
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "setup.h"
#include "ram.h"
#include "SPI.h"
#include "rom.h"

#define LED_OFF()	(DEBUG_PORT &= ~(1 << DEBUG_LED))
#define LED_ON()	(DEBUG_PORT |= (1 << DEBUG_LED))

#define ERROR_ON()	(ERROR_PORT |= (1 << ERROR_LED))
#define ERROR_OFF()	(ERROR_PORT &= ~(1 << ERROR_LED))

#define SIGNAL_READY()	(TIMING_PORT |= (1 << READY_PIN))
#define SIGNAL_STOP()	(TIMING_PORT |= (1 << STOP_PIN))
#define SIGNAL_START	(TIMING_PORT_IN & (1 << START_PIN))

void start_slave(void);
void stop_slave(void);
void wait_addrRange(uint16_t addrStart, uint16_t addrEnd);
void flash_led(uint8_t);

int main()
{
	setup();

	/* ROM to write to slave */
	uint8_t *ROM = (uint8_t*) malloc(sizeof(uint8_t) * 1024);

	/*Setup */
	// ROM[0x0000] = SEI;
	// ROM[0x0001] = CLD;
	// ROM[0x0002] = LDX_imm; ROM[0x0003] = 0x40;
	// ROM[0x0004] = STX_abs; ROM[0x0005] = 0x17; ROM[0x0006] = 0x40;
	// ROM[0x0007] = LDX_imm; ROM[0x0008] = 0xFF;
	// ROM[0x0009] = TXS;
	// ROM[0x000a] = INX;
	// ROM[0x000b] = STX_abs; ROM[0x000c] = 0x00; ROM[0x000d] = 0x20;
	// ROM[0x000e] = STX_abs; ROM[0x000f] = 0x01; ROM[0x0010] = 0x20;
	// ROM[0x0011] = STX_abs; ROM[0x0012] = 0x10; ROM[0x0013] = 0x40;

	/*Test simple store and load */
	// ROM[0x0014] = LDA_imm; ROM[0x0015] = 0xAA;
	// ROM[0x0016] = STA_zp; ROM[0x0017] = 0x20;  //zero page 0x20 should now contain 0xAA
	// ROM[0x0018] = LDA_imm; ROM[0x0019] = 0x55;
	// ROM[0x001a] = STA_zp; ROM[0x001b] = 0x21;  //zero page 0x21 should now contain 0x55
	// ROM[0x001c] = LDA_imm; ROM[0x001d] = 0x66;
	// ROM[0x001e] = STA_zp; ROM[0x001f] = 0x22;  //zero page 0x22 should now contain 0x66
	//
	/*Test logical operations */
	// ROM[0x0020] = LDA_zp; ROM[0x0021] = 0x20;
	// ROM[0x0022] = ORA_zp; ROM[0x0023] = 0x21;
	// ROM[0x0024] = STA_zp; ROM[0x0025] = 0x23;  //zero page 0x23 should now contain 0xFF
	// ROM[0x0026] = LDA_zp; ROM[0x0027] = 0x22;
	// ROM[0x0028] = EOR_imm; ROM[0x0029] = 0xFF;
	// ROM[0x002a] = STA_zp; ROM[0x002b] = 0x24;  //zero page 0x24 should now contain 0x99
	// ROM[0x002c] = LDA_zp; ROM[0x002d] = 0x23;
	// ROM[0x002e] = ORA_zp; ROM[0x002f] = 0x24;
	// ROM[0x0030] = ORA_imm; ROM[0x0031] = 0x24;
	// ROM[0x0032] = STA_zp; ROM[0x0033] = 0x25;  //zero page 0x25 should now contain 0xBD
	// ROM[0x0034] = STA_zp; ROM[0x0035] = 0x26;
	// ROM[0x0036] = ASL_zp; ROM[0x0037] = 0x26;  //zero page 0x26 should now contain 0x7A
	// ROM[0x0038] = LDA_zp; ROM[0x0039] = 0x26;
	// ROM[0x003a] = STA_zp; ROM[0x003b] = 0x27;
	// ROM[0x003c] = ASL_zp; ROM[0x003d] = 0x27;  //zero page 0x27 should now contain 0xF4

	/*Test arithmetic operations */
	// ROM[0x003e] = LDA_imm; ROM[0x003f] = 0x00;
	// ROM[0x0040] = ADC_imm; ROM[0x0041] = 0x01;
	// ROM[0x0042] = STA_zp; ROM[0x0043] = 0x28;  //zero page 0x28 should now contain 0x01
	// ROM[0x0044] = ADC_zp; ROM[0x0045] = 0x28;
	// ROM[0x0046] = STA_zp; ROM[0x0047] = 0x29;  //zero page 0x29 should now contain 0x02
	// ROM[0x0048] = ADC_zp; ROM[0x0049] = 0x29;
	// ROM[0x004a] = STA_zp; ROM[0x004b] = 0x2A;  //zero page 0x2A should now contain 0x04
	// ROM[0x004c] = ADC_zp; ROM[0x004d] = 0x2A;
	// ROM[0x004e] = STA_zp; ROM[0x004f] = 0x2B;  //zero page 0x2B should now contain 0x08
	// ROM[0x0050] = ADC_zp; ROM[0x0051] = 0x2B;
	// ROM[0x0052] = STA_zp; ROM[0x0053] = 0x2C;  //zero page 0x2C should now contain 0x10
	// ROM[0x0054] = STA_zp; ROM[0x0055] = 0x2D;
	// ROM[0x0056] = SBC_imm; ROM[0x0057] = 0x01;  //zero page 0x2D should now contain 0x0F
	// ROM[0x0058] = LDA_zp; ROM[0x0059] = 0x2D;
	// ROM[0x005a] = STA_zp; ROM[0x005b] = 0x2E;
	// ROM[0x005c] = SBC_imm; ROM[0x005d] = 0x01;
	// ROM[0x005e] = SBC_imm; ROM[0x005f] = 0x01;
	// ROM[0x0060] = SBC_imm; ROM[0x0061] = 0x01;
	// ROM[0x0062] = SBC_imm; ROM[0x0063] = 0x01;
	// ROM[0x0064] = SBC_imm; ROM[0x0065] = 0x01;  //zero page 0x2E should now contain 0x0A

	/*Test APU store and loads */
	// ROM[0x0066] = LDA_imm; ROM[0x0067] = 0x30;
	// ROM[0x0068] = STA_abs; ROM[0x0069] = 0x00; ROM[0x006a] = 0x40;  //0x4000 should now contain 0x30
	// ROM[0x006b] = LDA_imm; ROM[0x006c] = 0x00;
	// ROM[0x006d] = STA_abs; ROM[0x006e] = 0x01; ROM[0x006f] = 0x40;
	// ROM[0x0070] = STA_abs; ROM[0x0071] = 0x02; ROM[0x0072] = 0x40;
	// ROM[0x0073] = STA_abs; ROM[0x0074] = 0x03; ROM[0x0075] = 0x40;  //0x4001-0x4003 should now contain 0x00
	// ROM[0x0076] = LDA_abs; ROM[0x0077] = 0x00; ROM[0x0078] = 0x40;
	// ROM[0x0079] = STA_zp; ROM[0x007a] = 0x2F;  //zero page 0x2F should now (maybe) contain 0x30
	//
	/*Write zp values to RAM, from address 0x6000 and up */
	// ROM[0x007b] = LDA_zp; ROM[0x007c] = 0x20;
	// ROM[0x007d] = STA_abs; ROM[0x007e] = 0x00; ROM[0x007f] = 0x60;
	// ROM[0x0080] = LDA_zp; ROM[0x0081] = 0x21;
	// ROM[0x0082] = STA_abs; ROM[0x0083] = 0x01; ROM[0x0084] = 0x60;
	// ROM[0x0085] = LDA_zp; ROM[0x0086] = 0x22;
	// ROM[0x0087] = STA_abs; ROM[0x0088] = 0x02; ROM[0x0089] = 0x60;
	// ROM[0x008a] = LDA_zp; ROM[0x008b] = 0x23;
	// ROM[0x008c] = STA_abs; ROM[0x008d] = 0x03; ROM[0x008e] = 0x60;
	// ROM[0x008f] = LDA_zp; ROM[0x0090] = 0x24;
	// ROM[0x0091] = STA_abs; ROM[0x0092] = 0x04; ROM[0x0093] = 0x60;
	// ROM[0x0094] = LDA_zp; ROM[0x0095] = 0x25;
	// ROM[0x0096] = STA_abs; ROM[0x0097] = 0x05; ROM[0x0098] = 0x60;
	// ROM[0x0099] = LDA_zp; ROM[0x009a] = 0x26;
	// ROM[0x009b] = STA_abs; ROM[0x009c] = 0x06; ROM[0x009d] = 0x60;
	// ROM[0x009e] = LDA_zp; ROM[0x009f] = 0x27;
	// ROM[0x00a0] = STA_abs; ROM[0x00a1] = 0x07; ROM[0x00a2] = 0x60;
	// ROM[0x00a3] = LDA_zp; ROM[0x00a4] = 0x28;
	// ROM[0x00a5] = STA_abs; ROM[0x00a6] = 0x08; ROM[0x00a7] = 0x60;
	// ROM[0x00a8] = LDA_zp; ROM[0x00a9] = 0x29;
	// ROM[0x00aa] = STA_abs; ROM[0x00ab] = 0x09; ROM[0x00ac] = 0x60;
	// ROM[0x00ad] = LDA_zp; ROM[0x00ae] = 0x2A;
	// ROM[0x00af] = STA_abs; ROM[0x00b0] = 0x0A; ROM[0x00b1] = 0x60;
	// ROM[0x00b2] = LDA_zp; ROM[0x00b3] = 0x2B;
	// ROM[0x00b4] = STA_abs; ROM[0x00b5] = 0x0B; ROM[0x00b6] = 0x60;
	// ROM[0x00b7] = LDA_zp; ROM[0x00b8] = 0x2C;
	// ROM[0x00b9] = STA_abs; ROM[0x00ba] = 0x0C; ROM[0x00bb] = 0x60;
	// ROM[0x00bc] = LDA_zp; ROM[0x00bd] = 0x2D;
	// ROM[0x00be] = STA_abs; ROM[0x00bf] = 0x0D; ROM[0x00c0] = 0x60;
	// ROM[0x00c1] = LDA_zp; ROM[0x00c2] = 0x2E;
	// ROM[0x00c3] = STA_abs; ROM[0x00c4] = 0x0E; ROM[0x00c5] = 0x60;
	// ROM[0x00c6] = LDA_zp; ROM[0x00c7] = 0x2F;
	// ROM[0x00c8] = STA_abs; ROM[0x00c9] = 0x0F; ROM[0x00ca] = 0x60;

	/*Jump to end loop (0xFFF0) */
	// ROM[0x00cb] = JMP_abs; ROM[0x00cc] = 0xF0; ROM[0x00cd] = 0xFF;

	/*End of instructions */
	// ROM[0x00ce] = 0xFF; ROM[0x00cf] = 0xFF; ROM[0x00d0] = 0xFF;



	/***************** Main Program ***********************/

	volatile uint16_t i;
	for (i = 0; i < 1000; i++) {
		if (i % 2 == 0)
			ROM[i] = LDA_imm;
		else
			ROM[i] = 0x01;
	}
	ROM[i++] = JMP_abs;
	ROM[i++] = 0xF0;
	ROM[i++] = 0xFF;
	ROM[i++] = 0xFF;
	ROM[i++] = 0xFF;

	if (!ROM_TO_RAM(ROM)) ERROR_ON();
	while(1) {}

	uint32_t numInstr = 10;
	SIGNAL_READY();
	while (1) {

		/* Wait for start */
		while (!SIGNAL_START);

		/* Write ROM and indicate if success */
		if (!ROM_TO_RAM(ROM)) ERROR_ON();

		/* Start slave, and run program until it has reached end loop */
		start_slave();
		wait_addrRange(0xFFF0, 0xFFF2);
		stop_slave();

		SIGNAL_STOP();
		// LED_ON();
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

/* Waits until slave is within the address range addrStart - addrEnd */
void wait_addrRange(uint16_t addrStart, uint16_t addrEnd)
{
	volatile uint16_t readAddr;
	while(1) {
		readAddr = SPI_read(GPIOA) << 8;
		readAddr |= SPI_read(GPIOB);
		readAddr |= 0x8000;
		if (readAddr >= addrStart && readAddr <= addrEnd)  {
			break;
		}
	}
}
