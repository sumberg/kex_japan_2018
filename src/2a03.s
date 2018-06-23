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
LATCH_CTRL_OE_BIT = 0x02

;; I/O ports
LATCH_CTRL_PORT = 0x03 ;; PORTB
DATA_OUT = 0x0B ;; PORTD
DATA_IN = 0x0B;; PORTD for now
RWPORT = 0x03 ;; PORTB
TCCR0B = 0x25
TCNT0 = 0x26
DDRD = 0x0A

;; Masks
DIR_INPUT = 0x00
DIR_OUTPUT = 0xFF

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
.global test_invert12
.global test_invert15
.global test_invert16
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

	;; Wait for R/W to tranisiton from low to high
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
;;; FETCH_SLAVE_DATA
;;; 
;;; Tells slave to write contents of accumulator to memory, with STA_zp, and
;;; gets data output to data bus by slave during the slaves write cycle. This
;;; routine will tri-state the latch while reading data, in order to not pollute
;;; output data with whatever lies on the data bus from the master.

.macro FETCH_SLAVE_DATA fill
	;; Put STA_zp instruction on bus
	;; also multitask and prepare latch writing, port direction etc
	ldi r18, STA_zp
	in r20, LATCH_CTRL_PORT ;; keep this to restore later
	mov r21, r20
	ori r21, LATCH_CTRL_OE_BIT ;; use to set latch as tri-state
	ldi r22, DIR_INPUT ;; use to set data pins as input
	ldi r23, DIR_OUTPUT ;; use to set data pins as output
	nop
	nop
	nop
	nop
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18

	;; Wait for write signal, and then perform read within a few cycles (1/2 - 3/4 6502 cycle)
3:	sbic RWPORT, RWBIT
	rjmp 3b

	;; tri-state latch
	out LATCH_CTRL_PORT, r21

	;; change direction of PORTD (data bus)
	;; TODO only needed until DATA_IN is on its own 8 bit port
	out DDRD, r22

	;; read data into r16
	in r16, DATA_IN

	;; change direction of PORTD (data bus)
	;; TODO only needed until DATA_IN is on its own 8 bit port
	out DDRD, r23

	;; output enable latch by restoring previous port values
	out LATCH_CTRL_PORT, r20

	;; get in sync by waiting for high on r/w pin
4:	sbis RWPORT, RWBIT
	rjmp 4b

	;; Put STA_zp on bus
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r18
.endm


;;; ----------------------------------------------------------------------------
;;; TEST_INVERT
;;; 
;;; parameters:
;;;		r24 - val
;;; returns:
;;;		r24 - result
;;;
;;; TEST_INVERT will write val to the slave, the slave will invert it (bitwise XOR)
;;; and write it back on the data bus. The master will read the data bus during
;;; the slaves write cycle (one cycle window). Once the result is fetched, the master
;;; will invert it again and return it. If returned value is the same as argument value,
;;; the test is ok.

.macro TEST_INVERT fill
	;; Setup first write and SYNC
	ldi r20, LDA_imm
	SYNC

	;; LDA_imm opcode on data bus
	nop
	.rept \fill
	nop
	.endr
	out DATA_OUT, r20

	;; val on data bus
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
	out DATA_OUT, r24

	;; XOR step 1: put EOR_imm on bus
	ldi r20, EOR_imm
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

	;; XOR step 2: put 0xFF (to invert) on bus
	ldi r20, 0xFF
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

	;; FETCH_SLAVE_DATA gets result in r16, and leaves STA_zp on bus
	FETCH_SLAVE_DATA \fill

	;; Invert value again, and place result in return register
	com r16
	mov r24, r16
	ret
.endm

test_invert12:
	TEST_INVERT 0

test_invert15:
	TEST_INVERT 3

test_invert16:
	TEST_INVERT 4
