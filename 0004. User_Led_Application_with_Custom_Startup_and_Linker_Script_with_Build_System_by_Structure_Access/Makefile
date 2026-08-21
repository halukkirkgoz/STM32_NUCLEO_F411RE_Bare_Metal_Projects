# Define project files and tools
PROJECT = stm32f411re_led
BUILD_DIR = build
TARGET = $(BUILD_DIR)/$(PROJECT)
LINKER_SCRIPT = stm32f411retx_custom_linker_script.ld

CC = arm-none-eabi-gcc
SIZE = arm-none-eabi-size
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy
OPENOCD = openocd

# Keep all compiler flags in one place
CFLAGS = \
	-mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 \
	-mfloat-abi=soft -mthumb -std=gnu17 -g3 -DDEBUG \
	-DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx \
	-O0 -ffunction-sections -fdata-sections \
	-Wall -Wextra -pedantic -Wmissing-include-dirs \
	-Wswitch-default -Wswitch-enum -Wconversion \
	-fstack-usage --specs=nano.specs

# Keep all linker flags in one place
LDFLAGS = \
	-mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 \
	-mfloat-abi=soft -mthumb -T $(LINKER_SCRIPT) \
	--specs=nosys.specs --specs=nano.specs \
	-Wl,-Map=$(MAP) -Wl,--cref -Wl,--gc-sections \
	-static -u _printf_float

LDLIBS = -Wl,--start-group -lc -lm -Wl,--end-group

# Define generated files
MAIN_OBJ = $(BUILD_DIR)/main.o
STARTUP_OBJ = $(BUILD_DIR)/stm32f411retx_custom_startup.o
SYSCALLS_OBJ = $(BUILD_DIR)/syscalls.o
SYSMEM_OBJ = $(BUILD_DIR)/sysmem.o
OBJECTS = $(MAIN_OBJ) $(STARTUP_OBJ) $(SYSCALLS_OBJ) $(SYSMEM_OBJ)

ELF = $(TARGET).elf
BIN = $(TARGET).bin
HEX = $(TARGET).hex
LIST = $(TARGET).list
MAP = $(TARGET).map

OPENOCD_INTERFACE = interface/stlink.cfg
OPENOCD_TARGET = target/stm32f4x.cfg
FLASH_ADDRESS = 0x08000000

.PHONY: final clean rebuild flash-elf flash-bin

# Build ELF, BIN, HEX, LIST, and MAP files
final: $(ELF) $(BIN) $(HEX) $(LIST)
	@printf "\n"
	@printf "%s\n" "======================================================================"
	@printf "  %-10s %s\n" "Project" "$(PROJECT)"
	@printf "%s\n" "----------------------------------------------------------------------"
	@printf "  OUTPUT FILES\n"
	@printf "  %-10s %s\n" "ELF" "$(ELF)"
	@printf "  %-10s %s\n" "BIN" "$(BIN)"
	@printf "  %-10s %s\n" "HEX" "$(HEX)"
	@printf "  %-10s %s\n" "LIST" "$(LIST)"
	@printf "  %-10s %s\n" "MAP" "$(MAP)"
	@printf "%s\n" "----------------------------------------------------------------------"
	@printf "  MEMORY USAGE\n"
	@$(SIZE) $(ELF)
	@printf "%s\n" "======================================================================"

# Create the build directory
$(BUILD_DIR):
	@mkdir -p "$(BUILD_DIR)"

# Create an object file for main.c
$(MAIN_OBJ): main.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(BUILD_DIR)/main.d -MT $@ -c $< -o $@

# Create an object file for the startup code
$(STARTUP_OBJ): stm32f411retx_custom_startup.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(BUILD_DIR)/stm32f411retx_custom_startup.d -MT $@ -c $< -o $@

# Create an object file for system calls
$(SYSCALLS_OBJ): syscalls.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(BUILD_DIR)/syscalls.d -MT $@ -c $< -o $@

# Create an object file for memory management
$(SYSMEM_OBJ): sysmem.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(BUILD_DIR)/sysmem.d -MT $@ -c $< -o $@

# Link the object files to create the ELF file
$(ELF): $(OBJECTS)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Convert the ELF file to raw binary
$(BIN): $(ELF)
	@$(OBJCOPY) -O binary $< $@

# Convert the ELF file to Intel HEX
$(HEX): $(ELF)
	@$(OBJCOPY) -O ihex $< $@

# Create the disassembly and symbol list
$(LIST): $(ELF)
	@$(OBJDUMP) -h -S -t $< > $@

# Program the board using the ELF file
flash-elf: $(ELF)
	@printf "\n"
	@$(OPENOCD) -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "init" -c "reset halt" -c "program $(ELF) verify" -c "reset run" -c "shutdown"

# Program the board using the BIN file
flash-bin: $(BIN)
	@printf "\n"
	@$(OPENOCD) -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "init" -c "reset halt" -c "flash write_image erase $(BIN) $(FLASH_ADDRESS)" -c "verify_image $(BIN) $(FLASH_ADDRESS)" -c "reset run" -c "shutdown"

# Remove all generated files
clean:
	@rm -rf -- "$(BUILD_DIR)"

# Clean and build the project again
rebuild: clean
	@"$(MAKE)" --no-print-directory final

# Include automatically generated header dependencies
-include $(BUILD_DIR)/main.d
-include $(BUILD_DIR)/stm32f411retx_custom_startup.d
-include $(BUILD_DIR)/syscalls.d
-include $(BUILD_DIR)/sysmem.d
