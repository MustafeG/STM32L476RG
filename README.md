# STM32 Nucleo-L476RG Bare-Metal Drivers

Bare-metal firmware for the STM32 Nucleo-L476RG development board, written entirely without the STM32 HAL library. All peripheral access is done through direct register manipulation using CMSIS device headers.

## What this project is

This project is a foundation for writing low-level embedded drivers on the STM32L476RGT6 (Cortex-M4F) from scratch. No CubeIDE, no HAL, no abstraction layers — just your code, the CMSIS headers, and the hardware.

## What it does

The current firmware blinks the LD2 LED (PA5) on the Nucleo board by directly writing to the GPIO and RCC peripheral registers.

## Project structure

```
nucleo-l476rg/
├── cmake/
│   └── arm-none-eabi-gcc.cmake       # ARM cross-compilation toolchain file
├── Drivers/
│   └── CMSIS/
│       ├── Include/                  # ARM core headers (core_cm4.h, cmsis_gcc.h, etc.)
│       └── Device/ST/STM32L4xx/
│           ├── Include/              # ST chip headers (stm32l476xx.h, etc.)
│           └── Source/Templates/
│               └── gcc/
│                   └── startup_stm32l476xx.s
├── Inc/                              # Your own driver headers
├── Src/
│   └── main.c
├── .vscode/
│   ├── launch.json                   # Cortex-Debug configuration
│   └── settings.json                 # ARM toolchain path
├── CMakeLists.txt
├── STM32L476RGTX_FLASH.ld            # Linker script
├── STM32L476.svd                     # Peripheral register definitions for debugger
└── openocd.cfg                       # OpenOCD flash/debug configuration
```

## Building

Make sure the toolchain is installed and on your PATH before building. See [SETUP.md](SETUP.md) for full installation instructions.

### First time only

Run these once to create the build folder and configure CMake:

```
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
```

Re-run the `cmake` command if you ever add or remove source files in `CMakeLists.txt`, change a compiler or linker flag, or delete the `build` folder.

### Every time you change your code

From inside the `build` folder:

```
ninja
```

Ninja only recompiles files that changed, so this is fast.

### Full workflow from scratch

```
cd C:\projects\nucleo-l476rg
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
ninja
```

A successful build prints something like:

```
text    data     bss     dec     hex filename
 816       0       0     816     330 firmware.elf
```

## Flashing

Plug in the Nucleo board via USB, then from the project root:

```
openocd -f openocd.cfg -c "program build/firmware.elf verify reset exit"
```

A successful flash ends with:

```
** Verified OK **
** Resetting Target **
shutdown command invoked
```

## Day-to-day workflow

After the first-time setup, the typical cycle is:

1. Edit your code in VS Code
2. Rebuild from the `build` folder:
```
cd C:\projects\nucleo-l476rg\build
ninja
```
3. Flash from the project root:
```
cd C:\projects\nucleo-l476rg
openocd -f openocd.cfg -c "program build/firmware.elf verify reset exit"
```

That's it — edit, `ninja`, flash, repeat.

## Debugging

Open the project in VS Code:

```
code .
```

Set a breakpoint by clicking in the left margin of any line in `main.c`, then press **F5**. VS Code will flash the firmware and halt at `main()`. The left panel shows local variables, peripheral registers (via SVD), and the call stack.

## Hardware notes

- **MCU:** STM32L476RGT6 (Cortex-M4F, 80MHz max, 1024KB Flash, 128KB RAM)
- **LED:** LD2 connected to PA5 (requires JP6 jumper to be closed)
- **Debug interface:** ST-Link V2 onboard via SWD
- **Flash origin:** 0x08000000
- **RAM origin:** 0x20000000 (96KB SRAM1 used)