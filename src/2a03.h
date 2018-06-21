#pragma once

#include <stdint.h>
#include <util/atomic.h>

/* Writes a value to slave memory */
void slave_write(uint8_t, uint8_t,uint8_t val);
/* Disable interrupts on slave device */
void disable_slave_interrupts(void);
