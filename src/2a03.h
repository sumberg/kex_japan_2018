#pragma once

#include <stdint.h>
#include <util/atomic.h>
#include "rom.h"

/* Writes a value to slave memory */
void slave_write(uint8_t, uint8_t,uint8_t val);
/* Disable interrupts on slave device */
void disable_slave_interrupts(void);
/* Resets slave program counter to $8585 */
void reset_slave_pc(void);
/* Writes value to slave accumulator */
void write_slave_accumulator(uint8_t val);
/* Fetches slave accumulator data */
uint8_t fetch_slave_data(void);
/* Setup timing of slave unit, and functions to use for communicating */
void setup_slave_timing(void);
/* Inverts current value in slave accumulator */
void invert_slave_accumulator(void);
/* Sends instruction to slave device */
void send_slave_instruction(Instruction*);
