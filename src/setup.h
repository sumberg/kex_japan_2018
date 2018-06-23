#include <avr/io.h>

/* PORT direction bitmasks */
#define PORTB_SLAVE_RESET 0x01
#define PORTB_LATCH_OE 0x02
#define PORTB_CLOCK_IN 0x00; // added for readability, should be zero on PB6
#define PORTC_ALL_OUTPUT 0xFF
#define PORTD_ALL_OUTPUT 0xFF

#define SLAVE_RESET_MASK 0xFE;
#define SLAVE_RELEASE_MASK 0x01;
#define LATCH_OUTPUT_ENABLE_MASK 0xFC;
#define LATCH_OUTPUT_DISABLE_MASK 0x02;

/* Setup GPIO pins for input/output */
void setup_ports(void);
/* Sends (and holds) reset signal to slave */
void reset_slave(void);
/* Sends (and holds) release signal to slave. Used to undo reset/start slave. */
void release_slave(void);
/* Enable latch output */
void latch_output_enable(void);
/* Disable latch output (tri-state) */
void latch_output_disable(void);
/* Run entire setup routine, and leave slave in reset mode */
void setup(void);
