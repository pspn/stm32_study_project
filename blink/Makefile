# Makefile ปรับสำหรับ STM32L476RG
TARGET    = blink
CC        = arm-none-eabi-gcc
OBJCOPY   = arm-none-eabi-objcopy

CFLAGS    = -mcpu=cortex-m4 -mthumb -g -Og -ffreestanding -fno-common -nostdlib
LDFLAGS   = -T stm32l476rg.ld -Wl,--gc-sections

all: $(TARGET).bin

$(TARGET).elf: startup.o main.o 
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

startup.o: startup.s
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f *.o *.elf *.bin


