# Instruments

Instruments made on the tracker had to be translated into code that could run on the RP2A03 APU [referens till APU]. The two pulse channels (hereby referred to as PU1 and PU2 respectively) are made up of registers that are read as parameters to the instrument. The parameters that could be set for both PU1 and PU2 are identical, as can be seen in table [ref] below.

|-----------------------------------------------------------------------------|
|      CH     |              PU1              |              PU2              |
|-------------+-------------------------------+-------------------------------|
| Bits \ Addr | $4000 | $4001 | $4002 | $4003 | $4004 | $4005 | $4006 | $4007 |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      7      |   D   |   E   | $L_2$ | $L_3$ |   D   |   E   | $L_6$ | $L_7$ |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      6      |   D   |   P   | $L_2$ | $L_3$ |   D   |   P   | $L_6$ | $L_7$ |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      5      | $L_0$ |   P   | $L_2$ | $L_3$ | $L_4$ |   P   | $L_6$ | $L_7$ |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      4      |   C   |   P   | $L_2$ | $L_3$ |   C   |   P   | $L_6$ | $L_7$ |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      3      | $N_0$ | $N_1$ | $L_2$ | $L_3$ | $N_4$ | $N_1$ | $L_6$ | $L_7$ |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      2      | $N_0$ |   S   | $L_2$ |   H   | $N_4$ |   S   | $L_6$ |   H   |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      1      | $N_0$ |   S   | $L_2$ |   H   | $N_4$ |   S   | $L_6$ |   H   |
|-------------+-------+-------+-------+-------+-------+-------+-------+-------|
|      0      | $N_0$ |   S   | $L_2$ |   H   | $N_4$ |   S   | $L_6$ |   H   |
|-----------------------------------------------------------------------------|

D: Duty cycle
$L_0$: Loop envelope/Disable length counter
C: Constant volume
$N_0$: Envelope period volume

# How to send PU1 and PU2 instruments to RP2A03

Once the RP2A03 reads special values on its data-pins, that indicate that a transfer of instrument data is about to happen, it should initiate a process as described by this pseudocode snippet:

```{C}
	readByte = recv();
	if (readByte == PU1 || readByte == PU2) {
		instrument i;
		end = false;
		while (!end) {
			i.duty_envl = recv_ack(DUTY_ENV);	
			i.sweep = recv_ack(SWEEP);	
			i.tmr_lo = recv_ack(TMR_LO);	
			i.tmr_hi_lencount = recv_ack(TMR_HI);	
			end = recv_ack(END);
		}
		if (readByte == PU1)
			set_pu1(i);
		else
			set_pu2(i);
	}
```

Where `recv()` just reads the data pins, and `recv_ack()` strives to communicate with an acknowledge based communication, posting what context the read byte was used in.
