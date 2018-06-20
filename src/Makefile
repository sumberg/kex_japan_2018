# Target name:
TARGET = nesizer_fw

# Target type
MCU=atmega328p
PROGRAMMER=usbasp
F_CPU=20000000L

MODULES = ./
CSRC = $(foreach m, $(MODULES), $(wildcard $(m)*.c))
OBJ = $(CSRC:.c=.o) # add 2a03 object file here later (2a03_s.o)

# Compiler / Linker flags:
CFLAGS = -mmcu=$(MCU) -Wall -O3 -std=gnu11 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -I$(shell pwd) -DTARGET
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map

# Fuses (All fuses except lfuse should be default)
LFUSE = 0xE0

###################################

.PHONY: compile flash clean

compile: $(TARGET).hex

flash: compile
	avrdude -c $(PROGRAMMER) -P usb -p $(MCU) -B 1 -U lfuse:w:$(LFUSE):m
	avrdude -c $(PROGRAMMER) -P usb -p $(MCU) -B 1 -U flash:w:$(TARGET).hex

clean:
	rm -f $(OBJ) $(TARGET).{hex,map,elf}

.SECONDARY: $(OBJS)

###################################

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.elf: $(OBJ)
	avr-gcc $^ $(LDFLAGS) -o $@

%.o : %.c
	avr-gcc $(CFLAGS) -c $< -o $@

-include $(CSRC:%.c=%.d)

2a03_s.o : io/2a03.s
	avr-as -mmcu=$(MCU) -c $< -o $@
