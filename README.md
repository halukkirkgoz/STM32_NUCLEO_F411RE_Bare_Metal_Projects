# STM32 Development Tools Setup for Windows OS

This guide walks through setting up a complete standalone development environment for STM32 microcontrollers on a Windows PC.

The environment does **not** require STM32CubeIDE or Microsoft Visual Studio Build Tools.

The development environment uses:

* Visual Studio Code
* Arm GNU Toolchain for `arm-none-eabi`
* xPack Windows Build Tools
* xPack LLVM Clang
* xPack OpenOCD
* ST-LINK USB drivers
* VS Code C/C++ Extension Pack extension
* VS Code Cortex-Debug extension

The primary compiler used to build STM32 firmware is:

```text
arm-none-eabi-gcc
```

`arm-none-eabi-gcc` is part of the **Arm GNU Toolchain** and is therefore not installed separately.

xPack LLVM Clang is primarily used for development tools such as:

```text
clang
clang-format
clang-tidy
```

xPack Windows Build Tools provides GNU Make and several Unix-like command-line utilities commonly used by embedded build systems.

xPack OpenOCD provides the GDB server and debug-probe communication layer used for flashing and debugging STM32 devices.

As of **August 16, 2026**, the versions used in this guide are:

| Tool                      |   Version |
| ------------------------- | --------: |
| Visual Studio Code        |   1.133.0 |
| Arm GNU Toolchain         | 15.3.Rel1 |
| xPack Windows Build Tools |   4.4.1-4 |
| xPack LLVM Clang          |  21.1.8-1 |
| xPack OpenOCD             |  0.12.0-7 |

> This guide assumes a **64-bit x86-64 Windows 10 or Windows 11 PC**.

---

# Step 1: Update Windows

Before installing the development tools, make sure Windows is fully updated.

Open:

```text
Settings
└── Windows Update
    └── Check for updates
```

Install all available required updates and restart Windows if necessary.

You can verify the operating system architecture from PowerShell:

```powershell
Get-CimInstance Win32_OperatingSystem | Select-Object Caption, Version, OSArchitecture
```

The expected architecture for this guide is:

```text
64-bit
```

---

# Step 2: Install Visual Studio Code

Visual Studio Code is used as the main source-code editor and development environment.

Official website:

[Visual Studio Code](https://code.visualstudio.com/)

Official Windows installation documentation:

[Installing Visual Studio Code on Windows](https://code.visualstudio.com/docs/setup/windows)

As of August 16, 2026, the current stable release is:

```text
Visual Studio Code 1.133.0
```

## 2.1. Download Visual Studio Code

Download the **Windows x64 User Installer** from the official Visual Studio Code website.

The User Installer is recommended for a normal single-user development workstation.

By default, VS Code is installed under:

```text
%LOCALAPPDATA%\Programs\Microsoft VS Code
```

For example:

```text
C:\Users\<USERNAME>\AppData\Local\Programs\Microsoft VS Code
```

The installer normally adds the `code` command to the Windows `PATH`.

After installation, close all existing PowerShell, Command Prompt, and Windows Terminal sessions.

Open a new PowerShell terminal.

## 2.2. Verify Visual Studio Code

Run:

```powershell
code --version
```

You should see the installed Visual Studio Code version.

Also verify the executable location:

```powershell
where.exe code
```

---

# Step 3: Install xPack Windows Build Tools

xPack Windows Build Tools provides Windows-native versions of GNU Make and several command-line utilities commonly required by embedded build systems.

Official website:

[xPack Windows Build Tools](https://xpack-dev-tools.github.io/windows-build-tools-xpack/)

Official installation guide:

[xPack Windows Build Tools Installation Guide](https://xpack-dev-tools.github.io/windows-build-tools-xpack/docs/install/)

The package provides tools including:

```text
make
busybox
sh
cp
rm
mkdir
echo
```

These tools are used for **build orchestration**.

They do not compile STM32 firmware.

The STM32 compiler is provided separately by the Arm GNU Toolchain.

As of this guide, the latest manually distributed xPack Windows Build Tools binary package is:

```text
4.4.1-4
```

The corresponding xpm package revision is:

```text
4.4.1-4.1
```

The underlying GNU Make version is:

```text
GNU Make 4.4.1
```

## 3.1. Download xPack Windows Build Tools

Download the Windows x64 archive:

```text
xpack-windows-build-tools-4.4.1-4-win32-x64.zip
```

from the official xPack Windows Build Tools releases.

The `win32-x64` suffix is xPack's platform naming convention and refers to 64-bit Windows.

## 3.2. Extract xPack Windows Build Tools

Create:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-windows-build-tools-4.4.1-4
```

Extract the archive into this directory.

The resulting installation should look like:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-windows-build-tools-4.4.1-4\
├── bin
├── distro-info
├── include
├── share
└── README.md
```

The executable directory is:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-windows-build-tools-4.4.1-4\bin
```

For example:

```text
C:\Users\<USERNAME>\AppData\Roaming\Embedded Tools\xPacks\xpack-windows-build-tools-4.4.1-4\bin
```

## 3.3. Add xPack Windows Build Tools to PATH

Open:

```text
Settings
└── System
    └── About
        └── Advanced system settings
            └── Environment Variables
```

Under **User variables**, edit:

```text
Path
```

Add:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-windows-build-tools-4.4.1-4\bin
```

Close all terminals and restart Visual Studio Code.

## 3.4. Verify xPack Windows Build Tools

Open a new PowerShell terminal and run:

```powershell
make --version
```

The output should report:

```text
GNU Make 4.4.1
```

Verify the executable location:

```powershell
where.exe make
```

Also verify the other tools:

```powershell
where.exe sh
where.exe cp
where.exe rm
where.exe mkdir
where.exe busybox
```

You can test BusyBox with:

```powershell
busybox --help
```

---

# Step 4: Install Arm GNU Toolchain

The Arm GNU Toolchain provides the compiler, assembler, linker, debugger, and binary utilities required for bare-metal STM32 development.

Official Arm GNU Toolchain page:

[Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

Starting with **15.3.Rel1**, new Arm GNU Toolchain releases are published through the Arm GitLab toolchain repository.

For STM32 Cortex-M development, the required target is:

```text
arm-none-eabi
```

This target is intended for bare-metal Arm applications without a hosted operating system.

The most important binaries are:

```text
arm-none-eabi-gcc
arm-none-eabi-g++
arm-none-eabi-as
arm-none-eabi-ld
arm-none-eabi-gdb
arm-none-eabi-ar
arm-none-eabi-nm
arm-none-eabi-objcopy
arm-none-eabi-objdump
arm-none-eabi-readelf
arm-none-eabi-size
```

As of this guide, the Arm GNU Toolchain release used is:

```text
15.3.Rel1
```

---

## 4.1. Download Arm GNU Toolchain

Open the official Arm GNU Toolchain release repository and select:

```text
15.3.Rel1
```

For a Windows x86-64 PC and STM32 bare-metal development, select the package for:

```text
Host:   Windows x86_64
Target: arm-none-eabi
```

The Windows archive is named:

```text
arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi.zip
```

Do **not** select:

```text
arm-none-linux-gnueabihf
```

or:

```text
aarch64-none-linux-gnu
```

Those targets are intended for GNU/Linux systems running on Arm processors.

For STM32 Cortex-M bare-metal firmware, use:

```text
arm-none-eabi
```

---

## 4.2. Extract Arm GNU Toolchain

Create a directory such as:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\ArmGNU
```

Extract the toolchain archive into this directory.

The resulting directory should look similar to:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\ArmGNU\
└── arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi\
    ├── arm-none-eabi
    ├── bin
    ├── include
    ├── lib
    ├── libexec
    ├── share
    ├── .version
    └── manifest
```

The compiler binaries are located in:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\ArmGNU\arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi\bin
```

---

## 4.3. Add Arm GNU Toolchain to PATH

Open:

```text
Settings
└── System
    └── About
        └── Advanced system settings
            └── Environment Variables
```

Under **User variables**, edit:

```text
Path
```

Add:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\ArmGNU\arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi\bin
```

Close all terminals and restart Visual Studio Code.

---

## 4.4. Verify Arm GNU Toolchain

Run:

```powershell
arm-none-eabi-gcc --version
```

Then verify the remaining important tools:

```powershell
arm-none-eabi-g++ --version
arm-none-eabi-gdb --version
arm-none-eabi-as --version
arm-none-eabi-ld --version
arm-none-eabi-objcopy --version
arm-none-eabi-objdump --version
arm-none-eabi-readelf --version
arm-none-eabi-size --version
```

Verify which compiler Windows resolves:

```powershell
where.exe arm-none-eabi-gcc
```

Also verify GDB:

```powershell
where.exe arm-none-eabi-gdb
```

The paths should point to:

```text
C:\Users\<USERNAME>\AppData\Roaming\Embedded Tools\ArmGNU\arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi\bin\arm-none-eabi-gdb.exe
```

---

# Step 5: Install xPack LLVM Clang

xPack LLVM Clang provides a standalone Windows distribution of LLVM/Clang.

Official website:

[xPack LLVM Clang](https://xpack-dev-tools.github.io/clang-xpack/)

Official installation guide:

[xPack LLVM Clang Installation Guide](https://xpack-dev-tools.github.io/clang-xpack/docs/install/)

In this development environment, the main STM32 firmware compiler remains:

```text
arm-none-eabi-gcc
```

xPack LLVM Clang is primarily installed to provide development and static-analysis tools such as:

```text
clang
clang++
clang-format
clang-tidy
llvm-ar
llvm-nm
llvm-objcopy
llvm-objdump
llvm-readelf
llvm-size
```

The recommended architecture is:

```text
Source Code
    │
    ├── clang-format
    │      └── source formatting
    │
    ├── clang-tidy
    │      └── static analysis
    │
    ▼
arm-none-eabi-gcc
    │
    ▼
STM32 Firmware
```

As of this guide, the current xPack LLVM Clang binary release is:

```text
21.1.8-1
```

The corresponding xpm package revision is:

```text
21.1.8-1.1
```

The underlying LLVM Clang version is:

```text
21.1.8
```

---

## 5.1. Download xPack LLVM Clang

Download:

```text
xpack-clang-21.1.8-1-win32-x64.zip
```

from the official xPack LLVM Clang releases.

The `win32-x64` suffix refers to Windows x86-64.

---

## 5.2. Extract xPack LLVM Clang

Create:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-clang-21.1.8-1
```

Extract the archive into this directory.

The resulting directory should be:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-clang-21.1.8-1\
├── bin
├── distro-info
├── include
├── lib
├── libexec
└── share
```

The executable directory is:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-clang-21.1.8-1\bin
```

---

## 5.3. Add xPack LLVM Clang to PATH

Add:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-clang-21.1.8-1\bin
```

to the Windows **User PATH**.

Close all terminals and restart Visual Studio Code.

---

## 5.4. Verify xPack LLVM Clang

Run:

```powershell
clang --version
```

The output should contain:

```text
xPack MinGW-w64 x86_64 clang version 21.1.8
```

Verify the remaining development tools:

```powershell
clang-format --version
clang-tidy --version
```

Verify executable locations:

```powershell
where.exe clang
where.exe clang-format
where.exe clang-tidy
```

---

# Step 6: Install xPack OpenOCD

OpenOCD, the Open On-Chip Debugger, is used for programming and debugging STM32 microcontrollers through SWD or JTAG.

Official xPack OpenOCD website:

[xPack OpenOCD](https://xpack-dev-tools.github.io/openocd-xpack/)

Official installation guide:

[xPack OpenOCD Installation Guide](https://xpack-dev-tools.github.io/openocd-xpack/docs/install/)

OpenOCD provides:

* GDB server functionality
* SWD communication
* JTAG communication
* Flash programming
* Breakpoint support
* Watchpoint support
* Target reset control
* Debug-probe configuration

The typical debugging path is:

```text
VS Code
    │
    ▼
Cortex-Debug
    │
    ▼
arm-none-eabi-gdb
    │
    ▼
xPack OpenOCD
    │
    ▼
 ST-LINK
    │
    ▼
STM32 MCU
```

As of this guide, the latest xPack OpenOCD binary release is:

```text
0.12.0-7
```

The corresponding xpm package revision is:

```text
0.12.0-7.1
```

---

## 6.1. Download xPack OpenOCD

Download:

```text
xpack-openocd-0.12.0-7-win32-x64.zip
```

from the official xPack OpenOCD releases.

---

## 6.2. Extract xPack OpenOCD

Create:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-openocd-0.12.0-7
```

Extract the archive into this directory.

The resulting structure should resemble:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-openocd-0.12.0-7\
├── bin
├── distro-info
├── openocd
└── README.md
```

The executable directory is:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-openocd-0.12.0-7\bin
```

---

## 6.3. Add xPack OpenOCD to PATH

Add:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-openocd-0.12.0-7\bin
```

to the Windows **User PATH**.

Close all terminals and restart Visual Studio Code.

---

## 6.4. Verify xPack OpenOCD

Run:

```powershell
openocd --version
```

The output should contain:

```text
xPack Open On-Chip Debugger 0.12.0+dev-02228-ge5888bda3-dirty (2025-10-04-22:44)
```

Verify the executable location:

```powershell
where.exe openocd
```

The executable should resolve to:

```text
%USERPROFILE%\AppData\Roaming\Embedded Tools\xPacks\xpack-openocd-0.12.0-7\bin\openocd.exe
```

---

# Step 7: Install the ST-LINK USB Driver

When using an ST-LINK/V2 or ST-LINK/V2-1 debug probe, install the official STMicroelectronics ST-LINK USB driver.

Official package:

```text
STSW-LINK009
```

Official page:

[STSW-LINK009](https://www.st.com/en/development-tools/stsw-link009.html)

The driver supports:

```text
ST-LINK/V2
ST-LINK/V2-1
STLINK-V3
```

and related STM32:

```text
Nucleo
Discovery
Evaluation
```

boards.

The driver exposes ST-LINK interfaces such as:

```text
ST Debug
Virtual COM Port
ST Bridge
```

For ST-LINK/V2 and ST-LINK/V2-1 devices, install the driver before connecting the probe when possible.

---

## 7.1. Install STSW-LINK009

Download the latest:

```text
STSW-LINK009
```

package from STMicroelectronics.

Extract the package.

For a 64-bit Windows system, run the appropriate AMD64 driver installer with administrator privileges.

After installation:

1. Disconnect the ST-LINK.
2. Restart Windows if necessary.
3. Connect the ST-LINK or STM32 development board.
4. Open Device Manager.
5. Verify that the ST-LINK interface is detected correctly.

---

## 7.2. Do Not Replace the ST-LINK Driver Unnecessarily

Do not replace the ST-LINK USB driver with Zadig or another generic WinUSB configuration unless a specific debug probe requires it.

Using a generic libusb/WinUSB driver can prevent other STMicroelectronics tools from communicating with the probe.

For ST-LINK probes, prefer the manufacturer's driver.

---

# Step 8: Configure Visual Studio Code

The recommended Visual Studio Code configuration uses:

```text
C/C++ Extension Pack
```

for C/C++ source-code intelligence, code navigation, IntelliSense, and CMake integration, and:

```text
Cortex-Debug
```

for STM32 debugging.

The Microsoft C/C++ extension provides its own IntelliSense engine. A separate `clangd` installation or VS Code `clangd` extension **is not required** for this configuration.

---

## 8.1. Install the C/C++ Extension Pack Extension

Install the official Microsoft C/C++ Extension Pack.

Extension identifier:

```text
ms-vscode.cpptools-extension-pack
```

Install it from PowerShell:

```powershell
code --install-extension ms-vscode.cpptools-extension-pack
```

The extension pack currently includes:

* C/C++
* C/C++ Themes
* C/C++ DevTools
* CMake Tools

The core C/C++ extension provides language support for C and C++, including IntelliSense and debugging integration. For embedded STM32 projects, Cortex-Debug is used as the primary debugging extension, while the C/C++ extension provides source-code intelligence.

Typical editor features include:

* Code completion
* Parameter information
* Syntax and semantic highlighting
* Go to definition/declaration
* Find references
* Symbol navigation
* Hover information
* Code actions
* IntelliSense diagnostics
* C/C++ code browsing

No `clangd` executable is required for this configuration.

The important executable for IntelliSense is the **actual C/C++ compiler used to build the firmware**.

The C/C++ extension can query the configured compiler to determine compiler-specific predefined macros, system include directories, and target information.

---

## 8.2. Use compile_commands.json with the C/C++ Extension

The Microsoft C/C++ extension needs the actual compiler options used for each translation unit to accurately understand an embedded C/C++ project.

The preferred configuration is:

```text
compile_commands.json
```

A compilation database contains the compile command associated with each source file and allows the C/C++ extension to obtain project-specific information such as:

* Include directories
* Preprocessor definitions
* Target architecture options
* CPU-specific options
* Floating-point configuration
* C and C++ language standards
* Compiler-specific flags
* Forced includes
* Per-source-file compile options

The Microsoft C/C++ extension supports `compile_commands.json` directly through the `compileCommands` configuration property. If a matching entry exists for the active source file, the compile command from the database is used to configure IntelliSense for that translation unit.

For an STM32F407 project, a compile command may contain options such as:

```text
-mcpu=cortex-m4
-mthumb
-mfpu=fpv4-sp-d16
-mfloat-abi=hard
-DSTM32F407xx
-IInc
-IDrivers/CMSIS/Include
-std=gnu17
```

Without the correct compiler options, IntelliSense may incorrectly report errors.

---

## 8.3. Install Cortex-Debug

Install the Cortex-Debug extension.

Extension identifier:

```text
marus25.cortex-debug
```

Install it using:

```powershell
code --install-extension marus25.cortex-debug
```

Cortex-Debug integrates:

```text
VS Code
+
arm-none-eabi-gdb
+
OpenOCD
```

for Arm Cortex-M debugging.

The typical STM32 debug architecture becomes:

```text
Visual Studio Code
        │
        ▼
  Cortex-Debug
        │
        ▼
arm-none-eabi-gdb
        │
        ▼
  xPack OpenOCD
        │
        ▼
     ST-LINK
        │
        ▼
    SWD / JTAG
        │
        ▼
    STM32 MCU
```

---

# Step 9: Verify PATH Resolution

It is important to verify that Windows is resolving the intended executables, particularly if other development environments such as Git, MSYS2, MinGW, STM32CubeIDE, or older Arm toolchains are installed.

Run:

```powershell
where.exe code

where.exe make
where.exe sh

where.exe arm-none-eabi-gcc
where.exe arm-none-eabi-g++
where.exe arm-none-eabi-gdb

where.exe clang
where.exe clang-format
where.exe clang-tidy

where.exe openocd
```

Make sure the first resolved executable for each tool belongs to the expected installation.

If all commands execute successfully and resolve to the expected installation directories, the development environment is ready.

When multiple toolchain versions are installed, explicitly specify the expected versions through the project configuration.

For example:

```text
Makefile configuration
VS Code workspace settings
VS Code tasks.json
VS Code launch.json
CI configuration
Environment scripts
```

---

# Step 10: Recommended Installation Layout

A clean installation can use the following directory layout:

```text
C:\
│
└── Users
    └── <USERNAME>
        └── AppData
            └── Roaming
                └── Embedded Tools
                    ├── xPacks
                    │   ├── xpack-windows-build-tools-4.4.1-4
                    │   ├── xpack-clang-21.1.8-1
                    │   └── xpack-openocd-0.12.0-7
                    │
                    └── ArmGNU
                        └── arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi
```

Visual Studio Code is normally installed separately under:

```text
%LOCALAPPDATA%\Programs\Microsoft VS Code
```

---

# Step 11: Complete STM32 Development Architecture

The complete environment is:

```text
                         Windows PC
                             │
                             ▼
                    Visual Studio Code
                             │
             ┌───────────────┼───────────────┐
             │               │               │
             ▼               ▼               ▼
     C/C++ Extension       Build           Debug
             │               │               │
             │               │               ▼
             │               │         Cortex-Debug
             │               │               │
             │               │               ▼
             │               │       arm-none-eabi-gdb
             │               │               │
             │               │               ▼
             │               │        xPack OpenOCD
             │               │               │
             │               │               ▼
             │               │            ST-LINK
             │               │               │
             │               │               ▼
             │               │           SWD / JTAG
             │               │               │
             │               │               ▼
             │               │           STM32 MCU
             │               │
             │               ▼
             │     xPack Windows Build Tools
             │               │
             │               ▼
             │             make
             │               │
             │               ▼
             │       Arm GNU Toolchain
             │               │
             │               ▼
             │       arm-none-eabi-gcc
             │               │
             │               ▼
             │           ELF Firmware
             │               │
             │      ┌────────┼─────────┐
             │      │        │         │
             │      ▼        ▼         ▼
             │   objcopy   objdump    size
             │
             ▼
      xPack LLVM Clang
        ├── clang-format
        └── clang-tidy
```

---
