#define F_CPU 20000000UL
#include <util/delay.h>
#include "setup.h"
#include "ram.h"

int main(void)
{
	/* Setup system */
	setup();

	/* Test writes */
	RAM_write(0xA5, 0x72, 0x05);

	while(1) {
	}

	return 0;
}
