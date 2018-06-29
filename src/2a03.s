;;; This file is based on the same file from the NESIZER project
;;; with modifications on port usage to fit our project.
;;; The original license for this file is left as is below:
;;;
;;; Copyright (c) 2014-2015 Johan Fjeldtvedt
;;;
;;;	This file is part of NESIZER
;;;
;;; NESIZER is free software: you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation, either version 3 of the License, or
;;; (at your option) any later version.
;;;
;;; NESIZER is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.
;;;
;;; You should have received a copy of the GNU General Public License
;;; along with Foobar.  If not, see <http: //www.gnu.org/licenses/>.

;; Opcodes
LDA_imm = 0xA9
STA_abs = 0x8D
STA_zp = 0x85
JMP_abs = 0x4C
SEI = 0x78
EOR_imm = 0x49

;; R/W bit position on RWPORT
RWBIT = 0x01

;; I/O ports
RWPORT = 0x03			;; PORTB
DATA_OUT = 0x0B			;; PORTD
DATA_IN = 0x06			;; PINC
PORTC = 0x08			;; PORTC

;; Masks
DATA_IN_3_LO_BITS = 0x07 ;; Take the lowest 4 bits
OE_DISABLE_MASK = 0x08 ;; HIGH for disable
OE_ENABLE_MASK = 0xF7 ;; LOW for enable

/* Timer used for detect */
TCCR0B = 0x25
TCNT0 = 0x26

;; Global function declarations here
.global slave_memory_write12
.global slave_memory_write15
.global slave_memory_write16
.global slave_disable_interrupts12
.global slave_disable_interrupts15
.global slave_disable_interrupts16
.global slave_reset_pc12
.global slave_reset_pc15
.global slave_reset_pc16
.global detect
.global slave_fetch_data
.global slave_write_accumulator12
.global slave_write_accumulator15
.global slave_write_accumulator16
.global detect_64

.section .text

;;; ----------------------------------------------------------------------------

;;; SYNC macro
;;;
;;; Waits for the R/W line to go low, then to go high. When this has happened
;;; the 6502 is just finished with the last cycle of its idle STA_zp
;;; instruction, and a new instruction can be put on the bus.

.macro SYNC
1:	sbic RWPORT, RWBIT
	rjmp 1b
2:	sbis RWPORT, RWBIT
	rjmp 2b
.endm

;;; ----------------------------------------------------------------------------

;;; MEMORY_WRITE
;;;
;;; Parameters:
;;;		r24: Address high byte
;;; 	r22: Address low byte
;;;		r20: Value
;;; Return:	none
;;;
;;; Puts a given value at a given memory address by writing the following
;;; instructions:
;;;
;;; LDA #<val>
;;; STA <address high><address low> (addr. high and low must be fed in the inverse order)
;;;
;;; The write is followed by an STA_zp to keep the 6502 running.

.macro MEMORY_WRITE fill
	;; Put LDA_imm opcode in register
	ldi r18, LDA_imm

	;; Sync before writing
	SYNC

	;; Put LDA_imm opcode on data bus
	;; Finish with double out to keep original NESizer instruction count
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18
	out DATA_OUT, r18

	;; Put LDA_imm value on data bus
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r20
	out DATA_OUT, r20

	;; Put STA_abs opcode on data bus
	ldi r18, STA_abs
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18
	out DATA_OUT, r18

	;; Put low byte on data bus
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r22
	out DATA_OUT, r22

	;; Write high byte
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r24
	out DATA_OUT, r24

	;; Write STA_zp
	ldi r18, STA_zp
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18
	out DATA_OUT, r18

	ret
.endm

;;; Pass argument 4 for padding (2A03 divides input clock by 16)
slave_memory_write12:
	MEMORY_WRITE 0

slave_memory_write15:
	MEMORY_WRITE 3

slave_memory_write16:
	MEMORY_WRITE 4


;;; ----------------------------------------------------------------------------
;;; DISABLE_INTERRUPTS
;;;
;;; Parameters: none
;;; Return: 	none
;;;
;;; Sends an SEI instruction followed by the idling STA_zp

.macro DISABLE_INTERRUPTS fill
	;; Put SEI instruction in r18
	ldi r18, SEI

	;; Sync before writing
	SYNC

	;; Put SEI instruction on data bus
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18
	out DATA_OUT, r18

	;; Write STA_zp
	ldi r18, STA_zp
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18
	out DATA_OUT, r18

	ret
.endm

slave_disable_interrupts12:
	DISABLE_INTERRUPTS 0

slave_disable_interrupts15:
	DISABLE_INTERRUPTS 3

slave_disable_interrupts16:
	DISABLE_INTERRUPTS 4

;;; ----------------------------------------------------------------------------
;;; RESET_PC
;;;
;;; Parameters: none
;;; Return:	none
;;;
;;; Resets the program counter by sending a JMP_abs opcode followed by just
;;; STA_zp. This makes the 6502 jump to address $8585 and continue 'storing' to
;;; $85.

.macro RESET_PC fill
	ldi r21, JMP_abs

	SYNC

	.rept \fill
	nop
	.endr
	out DATA_OUT, r21
	out DATA_OUT, r21

	;; Write STA_zp
	ldi r21, STA_zp
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r21
	out DATA_OUT, r21

	ret
.endm

slave_reset_pc12:
	RESET_PC 0

slave_reset_pc15:
	RESET_PC 3

slave_reset_pc16:
	RESET_PC 4

;;; ----------------------------------------------------------------------------
;;; detect -- 2A03 type auto detection
;;;
;;; Parameters: none
;;; Return:	r24: number of Atmega clock cycles per 6502 clock cycle
;;;
;;; When this function is run, the 6502 is being fed the STA instruction with
;;; absolute addressing, which takes four cycles. At its fourth cycle, it pulls
;;; the R/W line down. This can be used to measure how many clock cycles the
;;; Atmega has done for each cycle the 6502 has done by counting how many cycles
;;; it takes between each time R/W transitions from low to high. In this
;;; function the numer of Atmega cycles is counted for two such transitions to
;;; ensure that the timer is started and stopped at the exact same phase of the
;;; R/W transition. The counted number of cycles is divided by 8 to get the
;;; number of Atmega cycles per one 6502 cycle.

detect:
	ldi r18, 1
	ldi r19, 0
	out TCNT0, r19

	;; Wait for R/W to transition from low to high
	SYNC

	;; Start timer 0
	out TCCR0B, r18

	;; Wait for R/W to transition from low to high twice
	SYNC
	SYNC

	;; Stop timer 0
	out TCCR0B, r19

	;; Read timer count and divide by 8
	in r24, TCNT0
	lsr r24
	lsr r24
	lsr r24

	ret

;;; ----------------------------------------------------------------------------
;;; FETCH_DATA
;;;
;;; parameters: none
;;; returns: r24 - fetched value
;;;
;;; Assuming the slave is running STA_zp instruction, the contents of the
;;; accumulator will be written to zero page memory every third cycle.
;;; When the slave is in its third cycle (write), the R/W pin will go low,
;;; tri-stating the latch, and at this time we can read the value of the data
;;; pins.


slave_fetch_data:

	in r18, PORTC
	mov r19, r18
	ori r18, OE_DISABLE_MASK
 	andi r19, OE_ENABLE_MASK

	;; Wait for write signal, and then perform read within a few cycles (1/2 - 3/4 6502 cycle)
3:	sbic RWPORT, RWBIT
	rjmp 3b

	;; disable latch
	out PORTC, r18

	;; read data into r16
4:	in r16, DATA_IN
	sbis RWPORT, RWBIT
	rjmp 4b

	;; enable latch
	out PORTC, r19

	;; place value in return register
	andi r16, DATA_IN_3_LO_BITS
	mov r24, r16

	ret

;;;----------------------------------------------------------------------------
;;;	SLAVE_WRITE_ACCUMULATOR
;;;
;;;	Parameters: r24 - 1 Byte value to write to accumulator
;;; Returns: none
;;;
;;; Use this to put a value in accumulator. This value will continuously be
;;; held in accumulator until other routines are called.
;;;

.macro SLAVE_WRITE_ACCUMULATOR fill
	ldi r20, LDA_imm

	SYNC

	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r20

	mov r18, r24
	andi r18, 0xFF
	ori r18, 0x00
	mov r18, r24
	andi r18, 0xFF
	ori r18, 0x00
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18

	ldi r20, STA_zp
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r20
	ldi r24, 0x01

	ret
.endm

;; 0
slave_write_accumulator12:
	SLAVE_WRITE_ACCUMULATOR 0

;; 3
slave_write_accumulator15:
	SLAVE_WRITE_ACCUMULATOR 3

;; 4
slave_write_accumulator16:
	SLAVE_WRITE_ACCUMULATOR 4


detect_64:
	ldi r18, 1
	ldi r19, 0
	out TCNT0, r19

	;; Wait for R/W to transition from low to high
	SYNC

	;; Start timer 0
	out TCCR0B, r18

	;; Wait for R/W to transition from low to high twice
	SYNC
	SYNC

	;; Stop timer 0
	out TCCR0B, r19

	;; Read timer count and divide by 8
	in r24, TCNT0
	lsr r24
	lsr r24
	lsr r24

	ret
