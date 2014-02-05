###############################################################################
# Makefile for the project bootloader_bluetooth
###############################################################################

## General Flags
PROJECT = BluetoothBootloader
MCU = atmega162
TARGET = ../../build/bootloader_bluetooth.elf
CC = avr-gcc.exe

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -w -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF ../../build/dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=../../build/bootloader_bluetooth.map
LDFLAGS += -Wl,-section-start=.text=0x3800
LDFLAGS += -Wl,-section-start=.id2section=0x3ffe
LDFLAGS += -Wl,-section-start=.id1section=0x3ffc
LDFLAGS += -Wl,-section-start=.id0section=0x3ffa


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = bootloader_bluetooth.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ../../build/bootloader_bluetooth.hex ../../build/bootloader_bluetooth.eep ../../build/bootloader_bluetooth.lss size
bootloader_avr.o: ./bootloader_bluetooth.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) ../../build/bootloader_bluetooth.elf ../../build/dep ../../build/bootloader_bluetooth.hex ../../build/bootloader_bluetooth.eep ../../build/bootloader_bluetooth.lss ../../build/bootloader_bluetooth.map

## Other dependencies
-include $(shell mkdir ../../build/dep 2>/dev/null) $(wildcard ../../build/dep/*)

