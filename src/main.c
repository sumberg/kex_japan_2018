#define F_CPU 20000000UL
#include <util/delay.h>
#include "setup.h"
#include "2a03.h"

int main(void)
{
	/* Setup system */
	setup();

	/* Main loop */
	uint8_t test = 0;
	while(1) {
		_delay_ms(1000);
		PORTC &= ~(0x0F << 2);
		_delay_ms(1000);
		if (slave_test(test))
			PORTC |= (0x07 << 2);
		else
			PORTC |= (0x08 << 2);
		test++;
	}

	return 0;
}
