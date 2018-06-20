#include <avr/io.h>

#define PORTD_ALL_OUTPUT 0xFF
#define PORTB_SLAVE_RESET 0x01

#define SLAVE_RESET_MASK 0xFE;
#define SLAVE_RELEASE_MASK 0x01;

/* Setup GPIO pins for input/output */
void setup_ports(void);
/* Sends (and holds) reset signal to slave */
void reset_slave(void);
/* Sends (and holds) release signal to slave. Used to undo reset/start slave. */
void release_slave(void);
/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
