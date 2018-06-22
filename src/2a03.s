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

;; R/W bit position on RWPORT
RWBIT = 0x01

;; I/O ports
RWPORT = 0x03 ;; PORTB
DATA = 0x0B ;; PORTD
TCCR0B = 0x25
TCNT0 = 0x26

;; Global function declarations here
.global slave_memory_write12
.global slave_memory_write15
.global slave_memory_write16
.global slave_disable_interrupts12
.global slave_disable_interrupts15
.global slave_disable_interrupts16
.global detect
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
;;; STA <address high> <address low>
;;;
;;; The write is followed by an STA_zp to keep the 6502 running.

.macro MEMORY_WRITE fill
	;; Sync before writing
	SYNC

	;; Put LDA_imm opcode on data bus
	ldi r18, LDA_imm
	.rept \fill
	nop
	.endr
	out DATA, r18

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
	nop
	.rept \fill
	nop
	.endr
	out DATA, r20

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
	nop
	.rept \fill
	nop
	.endr
	out DATA, r18

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
	nop
	.rept \fill
	nop
	.endr
	out DATA, r22

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
	nop
	.rept \fill
	nop
	.endr
	out DATA, r24

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
	out DATA, r18
	out DATA, r18

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
	SYNC

	ldi r18, SEI
	.rept \fill
	nop
	.endr
	out DATA, r18

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
	nop
	.rept \fill
	nop
	.endr
	out DATA, r18

	ret
.endm

slave_disable_interrupts12:
	DISABLE_INTERRUPTS 0

slave_disable_interrupts15:
	DISABLE_INTERRUPTS 3

slave_disable_interrupts16:
	DISABLE_INTERRUPTS 4

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
