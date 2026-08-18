# STM32F411RE Bare-Metal LED Application with a Custom Build System

This project demonstrates how to build and flash a bare-metal user LED application for the **NUCLEO-F411RE** board. The application controls the on-board LD2 LED connected to **PA5** without STM32CubeIDE, STM32 HAL, or CMSIS device headers.

The project includes a custom C startup file, a custom linker script, Newlib system-call support, and a GNU Make build system. All generated files are stored in the `build/` directory.

## Project structure

```text
.
|-- main.c
|-- stm32f411retx_custom_startup.c
|-- stm32f411retx_custom_linker_script.ld
|-- syscalls.c
|-- sysmem.c
|-- Makefile
|-- README.md
`-- build/                               # Created automatically during the build
```

* `main.c`: Configures GPIOA pin 5 directly through memory-mapped registers and controls the on-board LED.
* `stm32f411retx_custom_startup.c`: Defines the interrupt vector table and reset sequence, initializes `.data` and `.bss`, runs the C library initializers, and calls `main()`.
* `stm32f411retx_custom_linker_script.ld`: Defines the STM32F411RE memory layout with 512 KiB of Flash and 128 KiB of SRAM.
* `syscalls.c`: Provides the basic low-level system calls required by Newlib.
* `sysmem.c`: Provides the `_sbrk()` implementation used by the Newlib heap.
* `Makefile`: Compiles, links, converts, inspects, cleans, rebuilds, and flashes the project.

## Application behavior

The application enables the GPIOA peripheral clock and configures PA5 as a low-speed, push-pull output with no pull-up or pull-down resistor.

The default build uses `LED_MODE_TOGGLE`, which repeatedly toggles LD2 with a simple busy-wait delay. The alternative `LED_MODE_CONST_ON` mode turns the LED on once and keeps it on.

To select the constant-on mode, add the following definition to `CFLAGS` in the Makefile:

```make
-DLED_MODE=0
```

The busy-wait loop is intended only for this introductory example. Its timing depends on compiler settings and processor clock speed, so it is not a precise delay source.

## Requirements

The following tools must be available through the `PATH` environment variable:

* Arm GNU Toolchain: `arm-none-eabi-gcc`, `arm-none-eabi-size`, `arm-none-eabi-objdump`, and `arm-none-eabi-objcopy`
* GNU Make
* A POSIX-compatible shell and basic commands such as `mkdir`, `rm`, and `printf` (the xPack Windows Build Tools environment provides these on Windows)
* OpenOCD
* A suitable USB driver for the on-board ST-LINK interface

Verify the main tools from the terminal:

```sh
arm-none-eabi-gcc --version
make --version
openocd --version
```

All commands in this document must be run from the **project root directory** containing the Makefile.

## Building the project

### Normal build

Run GNU Make without a target:

```sh
make
```

The first target in the Makefile is `final`, so the command above is equivalent to:

```sh
make final
```

The build system performs the following operations:

1. Creates the `build/` directory when it does not exist.
2. Compiles each C source file into a separate object file.
3. Generates header dependency (`.d`) and stack-usage (`.su`) files.
4. Links the objects with the custom linker script to create the ELF and MAP files.
5. Converts the ELF file into BIN and Intel HEX images.
6. Generates a disassembly and symbol listing.
7. Displays the output paths and a compact memory-usage summary.

GNU Make rebuilds only the files affected by a source or header change.

Compiling `syscalls.c` may produce a `-Wconversion` warning for the `int`-to-`char` conversion inside `_read()`. This warning does not stop the build.

### Clean rebuild

To delete the existing build output and compile everything again, run:

```sh
make rebuild
```

This target runs `clean` first and then invokes the `final` target.

## Build outputs

A successful build produces the following main files:

```text
build/
|-- main.o
|-- stm32f411retx_custom_startup.o
|-- syscalls.o
|-- sysmem.o
|-- main.d
|-- stm32f411retx_custom_startup.d
|-- syscalls.d
|-- sysmem.d
|-- main.su
|-- stm32f411retx_custom_startup.su
|-- syscalls.su
|-- sysmem.su
|-- stm32f411re_led.elf              # Executable with code and debug symbols
|-- stm32f411re_led.map              # Linker memory map and cross-reference table
|-- stm32f411re_led.list             # Sections, disassembly, source, and symbols
|-- stm32f411re_led.bin              # Raw Flash image
`-- stm32f411re_led.hex              # Intel HEX image
```

The ELF file is the primary build artifact. The BIN and HEX files are generated from it, while the LIST and MAP files are useful for inspecting the program.

## Flashing with OpenOCD

Connect the NUCLEO-F411RE board through its ST-LINK USB connector. Only one of the following flashing methods is required.

### Flash the ELF file (recommended)

```sh
make flash-elf
```

The ELF file contains its target address information, so OpenOCD can program it directly. The Makefile runs the equivalent of:

```sh
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "program build/stm32f411re_led.elf verify" -c "reset run" -c "shutdown"
```

### Flash the BIN file

```sh
make flash-bin
```

A BIN file contains raw bytes without address information. The Makefile therefore supplies the STM32 internal Flash base address, `0x08000000`:

```sh
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "flash write_image erase build/stm32f411re_led.bin 0x08000000" -c "verify_image build/stm32f411re_led.bin 0x08000000" -c "reset run" -c "shutdown"
```

Both targets build the required image first when it is missing or out of date. After programming, OpenOCD verifies the image, resets and starts the microcontroller, and then exits.

## Cleaning the build output

To remove the generated `build/` directory and all files inside it, run:

```sh
make clean
```

Source files, the linker script, the Makefile, and documentation are not removed. The next `make` command recreates the output directory automatically.

## Important compiler and linker options

* `-mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -mthumb`: Generates Thumb instructions tuned for the Cortex-M4 processor.
* `-mfloat-abi=soft`: Uses the software floating-point calling convention consistently during compilation and linking.
* `-std=gnu17`: Compiles the application using the GNU C17 language dialect.
* `-O0 -g3 -DDEBUG`: Produces an unoptimized development build with detailed debug information.
* `-DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx`: Defines the target board, device family, and MCU variant.
* `-Wall -Wextra -pedantic -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wconversion`: Enables strict compiler diagnostics.
* `-ffunction-sections -fdata-sections`: Places each function and data item in a separate section.
* `-Wl,--gc-sections`: Removes unused sections from the final ELF file.
* `-Wl,-Map=... -Wl,--cref`: Generates the linker MAP file and cross-reference table.
* `--specs=nano.specs`: Selects the smaller Newlib Nano implementation intended for embedded systems.
* `--specs=nosys.specs`: Supplies default system-call behavior for a bare-metal target without an operating system.
* `-static`: Links the application without shared libraries.
* `-u _printf_float`: Retains floating-point formatting support for `printf()`.
* `-MMD -MP`: Generates header dependency files without listing system headers.
* `-fstack-usage`: Generates a static stack-usage report for each source file.

---

> **Note:** Run all Make targets from the project root directory and ensure the NUCLEO-F411RE is connected through ST-LINK before using either flashing target.

---
