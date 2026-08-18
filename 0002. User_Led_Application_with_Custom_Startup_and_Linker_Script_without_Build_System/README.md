# STM32F411RE Bare-Metal LED Application

This project demonstrates how to build the user LED application for the **NUCLEO-F411RE** board (LD2, PA5) and flash it through ST-LINK without using STM32CubeIDE or any other build system.

The project uses a custom startup file and a custom linker script. All intermediate files and ELF/BIN/HEX outputs are stored in the `build/` directory.

## Project structure

```text
.
|-- main.c
|-- stm32f411retx_custom_startup.c
|-- stm32f411retx_custom_linker_script.ld
|-- syscalls.c
|-- sysmem.c
|-- README.md
|-- build/                               # Created during the building process

```

* `main.c`: Enables the GPIOA clock, configures PA5 as an output, and drives the user LED.
* `stm32f411retx_custom_startup.c`: Contains the vector table, reset sequence, `.data` copy, and `.bss` initialization.
* `stm32f411retx_custom_linker_script.ld`: Defines the STM32F411RE memory layout with 512 KiB of Flash and 128 KiB of SRAM.
* `syscalls.c`: Provides the basic system calls required by Newlib.
* `sysmem.c`: Provides the `_sbrk()` implementation used by the Newlib heap.

## Requirements

The following tools must be available through the `PATH` environment variable:

* Arm GNU Toolchain: `arm-none-eabi-gcc`, `arm-none-eabi-size`, `arm-none-eabi-objdump`, `arm-none-eabi-objcopy`
* OpenOCD
* A suitable USB driver for the on-board ST-LINK interface

Verify the installations in Command Prompt:

```cmd
arm-none-eabi-gcc --version
openocd --version
```

All commands must be run from the **project root directory** containing this README.

## Building the project

### 1. Create the output directory

```cmd
if exist build rmdir /S /Q build & mkdir build
```

This command deletes the existing `build/` directory (if it exists) with all its contents and creates a fresh, empty one; if it doesn't exist, it simply creates a new empty directory.

### 2. Compile the application source file

```cmd
arm-none-eabi-gcc main.c -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -std=gnu17 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -O0 -ffunction-sections -fdata-sections -Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wconversion -fstack-usage -MMD -MP -MF build/main.d -MT build/main.o --specs=nano.specs -mfloat-abi=soft -mthumb -o build/main.o
```

By default, `main.c` toggles the LED with a fixed delay. Add `-DLED_MODE=0` to this command to keep the LED continuously on instead.

### 3. Compile the custom startup file

The startup file in this project is a C (`.c`) file rather than an assembly (`.s`) file, so it is compiled as a regular C source file.

```cmd
arm-none-eabi-gcc stm32f411retx_custom_startup.c -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -std=gnu17 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -O0 -ffunction-sections -fdata-sections -Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wconversion -fstack-usage -MMD -MP -MF build/stm32f411retx_custom_startup.d -MT build/stm32f411retx_custom_startup.o --specs=nano.specs -mfloat-abi=soft -mthumb -o build/stm32f411retx_custom_startup.o
```

### 4. Compile the system calls

```cmd
arm-none-eabi-gcc syscalls.c -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -std=gnu17 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -O0 -ffunction-sections -fdata-sections -Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wconversion -fstack-usage -MMD -MP -MF build/syscalls.d -MT build/syscalls.o --specs=nano.specs -mfloat-abi=soft -mthumb -o build/syscalls.o
```

Compiling `syscalls.c` may produce a `-Wconversion` warning for the `int`-to-`char` conversion inside `_read()`. This is a warning and does not stop the build.

### 5. Compile the heap support

```cmd
arm-none-eabi-gcc sysmem.c -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -std=gnu17 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -O0 -ffunction-sections -fdata-sections -Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wconversion -fstack-usage -MMD -MP -MF build/sysmem.d -MT build/sysmem.o --specs=nano.specs -mfloat-abi=soft -mthumb -o build/sysmem.o
```

### 6. Link the object files

The `-Wl,...` arguments can be used directly in Command Prompt without quoting. The linker script is specified with a relative path, so the command remains valid if the project is moved to another directory.

```cmd
arm-none-eabi-gcc -o build/stm32f411re_led.elf build/main.o build/stm32f411retx_custom_startup.o build/syscalls.o build/sysmem.o -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -T stm32f411retx_custom_linker_script.ld --specs=nosys.specs --specs=nano.specs -Wl,-Map=build/stm32f411re_led.map -Wl,--cref -Wl,--gc-sections -static -mfloat-abi=soft -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
```

## Inspecting and converting the output

### Display detailed memory usage

```cmd
arm-none-eabi-size -A build/stm32f411re_led.elf
```

For a shorter Flash/RAM summary:

```cmd
arm-none-eabi-size build/stm32f411re_led.elf
```

### Generate the disassembly/list file

```cmd
arm-none-eabi-objdump -h -S -t build/stm32f411re_led.elf > build/stm32f411re_led.list
```

### Generate BIN and Intel HEX files

```cmd
arm-none-eabi-objcopy -O binary build/stm32f411re_led.elf build/stm32f411re_led.bin
arm-none-eabi-objcopy -O ihex build/stm32f411re_led.elf build/stm32f411re_led.hex
```

The main files produced by a successful build are:

```text
build/
|-- main.o
|-- stm32f411retx_custom_startup.o
|-- syscalls.o
|-- sysmem.o
|-- *.d                              # Header dependency files
|-- *.su                             # Stack-usage reports
|-- stm32f411re_led.elf              # Executable file with symbols
|-- stm32f411re_led.map              # Linker memory map
|-- stm32f411re_led.list             # Disassembly and symbol listing
|-- stm32f411re_led.bin              # Raw Flash image
`-- stm32f411re_led.hex              # Intel HEX image
```

## Flashing with OpenOCD

Connect the NUCLEO-F411RE board to the computer through its ST-LINK USB port. Only one of the following methods is required.

### Flash the ELF file (recommended)

The ELF file contains the target address information, so no explicit base address is required:

```cmd
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "program build/stm32f411re_led.elf verify" -c "reset run" -c "shutdown"
```

### Flash the BIN file

Because a BIN file contains no address information, the STM32F411RE internal Flash base address, `0x08000000`, must be supplied explicitly:

```cmd
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "flash write_image erase build/stm32f411re_led.bin 0x08000000" -c "verify_image build/stm32f411re_led.bin 0x08000000" -c "reset run" -c "shutdown"
```

After successful programming, OpenOCD verifies the image, resets and starts the microcontroller, and then exits.

## Cleaning the build output

The following command deletes only the `build/` directory in the project root and its generated contents:

```cmd
rmdir /S /Q build
```

Recreate the `build/` directory before the next build.

## Important compiler and linker options

* `-mcpu=cortex-m4 -mthumb`: Generates Thumb instructions for the Cortex-M4 processor.
* `-mfloat-abi=soft`: Uses the software floating-point calling convention; all source files and the link stage use the same ABI.
* `-O0 -g3 -DDEBUG`: Produces an unoptimized development build with detailed debug information.
* `-ffunction-sections -fdata-sections`: Places each function and data item in a separate section.
* `-Wl,--gc-sections`: Removes unused sections from the final ELF file.
* `--specs=nano.specs`: Selects the smaller Newlib Nano implementation intended for embedded systems.
* `--specs=nosys.specs`: Supplies default system-call behavior for a bare-metal target without an operating system.
* `-MMD -MP`: Generates a header dependency file (`.d`) for each source file.
* `-fstack-usage`: Generates a static stack-usage report (`.su`) for each source file.

---
