# Cross-compiler tools
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# Build flags
CFLAGS  = -mcpu=cortex-m4 -mthumb -O2 -ffreestanding -fdata-sections \
          -ffunction-sections -Wall -Wextra
LDFLAGS = -nostdlib -T LinkerScript.ld \
          -Wl,--gc-sections

# Targets
all: blink.bin

# Link the ELF
blink.elf: startup.s main.c LinkerScript.ld
	$(CC) $(CFLAGS) $(LDFLAGS) startup.s main.c -o $@
	$(SIZE) $@

# Generate loadable binary
blink.bin: blink.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.elf *.bin

.PHONY: all clean

