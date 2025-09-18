# ==== TOOLS ====
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy

# ==== DIRS ====
SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build

# ==== FILES ====
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES := $(wildcard $(SRC_DIR)/*.s)

C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJS := $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
OBJS := $(C_OBJS) $(ASM_OBJS)

ELF = $(BUILD_DIR)/firmware.elf
BIN = $(BUILD_DIR)/firmware.bin

# ==== FLAGS ====
CFLAGS = -mcpu=cortex-m4 -mthumb -ffreestanding -nostdlib -g -I$(LIB_DIR)
ASFLAGS = -mcpu=cortex-m4 -mthumb
LDFLAGS = -nostdlib -Wl,-Tlink.ld -Wl,--gc-sections

# ==== TARGETS ====
all: $(BIN)

# Link ELF
$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Convert to BIN
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Compile C
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# Create build dir
$(BUILD_DIR):
	mkdir -p $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
