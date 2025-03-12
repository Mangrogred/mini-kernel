CC = gcc
CFLAGS = -m32 -fno-pie -ffreestanding -fno-builtin -nostdlib -nodefaultlibs -Wall -Wextra -O0 -fno-stack-protector -I$(KERNEL_DIR)/include
ASM = nasm
ASMFLAGS = -f elf32
LD = ld
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

BUILD_DIR = build
BOOT_DIR = boot
KERNEL_DIR = kernel
DRIVERS_DIR = $(KERNEL_DIR)/drivers

KERNEL_C_SOURCES = $(KERNEL_DIR)/kernel.c $(wildcard $(DRIVERS_DIR)/*.c)
KERNEL_C_OBJECTS = $(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_C_SOURCES))

all: $(BUILD_DIR)/os.img

$(BUILD_DIR)/os.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@
	# Дополняем образ до размера флоппи-диска (1.44 МБ)
	dd if=/dev/zero bs=512 count=2880 >> $@
	truncate -s 1474560 $@

$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(KERNEL_C_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/kernel_entry.o: $(KERNEL_DIR)/kernel_entry.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BUILD_DIR)/os.img
	qemu-system-i386 -fda $<

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean 