#define F_CPU 20000000UL
#include <util/delay.h>
#include "setup.h"
#include "2a03.h"

int main(void)
{
	/* Setup system */
	setup();

	/* CODE BELOW LEFT TO TEST COMMUNICATION */

	uint8_t val = 0x01;
	uint8_t retVal;
	// write_slave_accumulator(0x06);

	/* Main loop */
	while(1) {
		write_slave_accumulator(val);
		invert_slave_accumulator();
		retVal = fetch_slave_data();
		for (int i = retVal; i > 0; i--) {
			PORTC |= (3 << 4);
			_delay_ms(500);
			PORTC &= ~(3 << 4);
			_delay_ms(500);
		}
		_delay_ms(2000);
		val = (val + 1) % 8;
	}

	return 0;
}
