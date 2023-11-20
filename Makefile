FIRMWARE=avr
OBJCOPY=avr-objcopy
DEVICE=atmega328p
SER_PORT=/dev/ttyUSB0
PROG_EXE=avrdude
PROG_PARAMS=-c arduino -P $(SER_PORT) -b 115200 -p $(DEVICE)
CC=avr-gcc
CFLAGS=-DF_CPU=16000000UL -Os -g3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mmcu=$(DEVICE)
LDFLAGS=-mmcu=$(DEVICE) -Xlinker -Map=$(FIRMWARE).map -Xlinker --gc-sections
SOURCES=main.c

OBJ = $(SOURCES:.c=.o)
RM=rm


upload:
	make all
	$(PROG_EXE) $(PROG_PARAMS) -U flash:w:$(FIRMWARE).hex:i

all: $(FIRMWARE).hex

$(FIRMWARE).hex:$(FIRMWARE).elf
	$(OBJCOPY) -O ihex $(FIRMWARE).elf $(FIRMWARE).hex

$(FIRMWARE).elf:$(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(FIRMWARE).elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

erase:
	$(PROG_EXE) $(PROG_PARAMS) -e

clean:
	$(RM) -rf *.elf  ./*~ ./$(OBJ) *.map *.hex 2 > /dev/null 