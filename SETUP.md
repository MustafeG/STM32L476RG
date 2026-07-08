# Development Environment Setup (Windows)

Complete installation guide for setting up a bare-metal STM32 development environment on Windows using VS Code, CMake, Ninja, and OpenOCD.

## Required tools

| Tool | Purpose |
|---|---|
| arm-none-eabi-gcc | ARM cross-compiler and linker |
| CMake | Build system configuration |
| Ninja | Build executor |
| OpenOCD | Flashing and debugging via ST-Link |
| VS Code | Editor and debug UI |
| ST-Link driver | USB driver for the Nucleo's onboard debugger |

---

## 1. ARM GNU Toolchain

Download the Windows zip from the ARM developer site:

https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

Look for the file named:
```
arm-gnu-toolchain-<version>-mingw-w64-x86_64-arm-none-eabi.zip
```

Extract it to a permanent location, for example:
```
C:\tools\arm-gnu-toolchain\
```

Inside you will find a `bin\` folder. Add it to your system PATH (see PATH configuration section below):
```
C:\tools\arm-gnu-toolchain\bin
```

Verify in a new terminal:
```
arm-none-eabi-gcc --version
```

---

## 2. CMake

Download the Windows x64 installer from:

https://cmake.org/download/

During installation, select **"Add CMake to the system PATH for all users"**.

Verify:
```
cmake --version
```

---

## 3. Ninja

Download `ninja-win.zip` from the latest release:

https://github.com/ninja-build/ninja/releases

Extract `ninja.exe` to:
```
C:\tools\ninja\
```

Add `C:\tools\ninja` to your system PATH (see PATH configuration section below).

Verify in a new terminal:
```
ninja --version
```

---

## 4. OpenOCD

Use the xPack OpenOCD release for Windows:

https://github.com/xpack-dev-tools/openocd-xpack/releases

Download the zip named:
```
xpack-openocd-<version>-win32-x64.zip
```

Extract to:
```
C:\tools\xpack-openocd\
```

Add `C:\tools\xpack-openocd\bin` to your system PATH.

Verify in a new terminal:
```
openocd --version
```

---

## 5. ST-Link USB driver

Without this driver, OpenOCD cannot communicate with the Nucleo board.

Download **STSW-LINK009** from ST's website:

https://www.st.com/en/development-tools/stsw-link009.html

Run the installer, then plug in your Nucleo board. Open **Device Manager** and confirm you see:

```
Universal Serial Bus devices
└── ST-Link Debug
```

If you see an unknown device with a yellow warning icon, the driver did not install correctly.

---

## 6. VS Code extensions

Open VS Code and install these three extensions from the Extensions panel (Ctrl+Shift+X):

| Extension | Publisher | Purpose |
|---|---|---|
| C/C++ | Microsoft | IntelliSense and code navigation |
| CMake Tools | Microsoft | CMake integration in VS Code |
| Cortex-Debug | marus25 | ARM debugging with OpenOCD and GDB |

---

## PATH configuration

PATH is an environment variable that tells Windows which folders to search when you run a command. Tools like `ninja`, `cmake`, and `arm-none-eabi-gcc` must be on PATH so they can be found from the terminal and by VS Code.

To add a folder to PATH:

1. Press `Win + S` and search for **"Edit the system environment variables"**
2. Click **Environment Variables**
3. Under **System variables**, find the variable named **Path** and click **Edit**
4. Click **New** and add the folder path
5. Click OK all the way out
6. Open a **new** terminal — existing terminals do not pick up PATH changes

Folders to add:
```
C:\tools\arm-gnu-toolchain\bin
C:\tools\ninja
C:\tools\xpack-openocd\bin
```

CMake adds itself automatically if you select the option during installation.

---

## Verify all tools

Open a fresh terminal and run each of these:

```
arm-none-eabi-gcc --version
cmake --version
ninja --version
openocd --version
```

All four should print version information without errors.

---

## CMSIS headers

The project requires two sets of headers — ARM core headers and ST device headers. Neither requires installing STM32CubeIDE.

**ARM CMSIS core headers:**

https://github.com/ARM-software/CMSIS_5

Download the zip, navigate to `CMSIS_5-develop\CMSIS\Core\Include\` and copy all files into:
```
Drivers\CMSIS\Include\
```

**ST device headers:**

https://github.com/STMicroelectronics/cmsis_device_l4

Download the zip and copy files as follows:

| Source | Destination |
|---|---|
| `Include\stm32l476xx.h` | `Drivers\CMSIS\Device\ST\STM32L4xx\Include\` |
| `Include\stm32l4xx.h` | `Drivers\CMSIS\Device\ST\STM32L4xx\Include\` |
| `Include\system_stm32l4xx.h` | `Drivers\CMSIS\Device\ST\STM32L4xx\Include\` |
| `Source\Templates\system_stm32l4xx.c` | `Drivers\CMSIS\Device\ST\STM32L4xx\Source\Templates\` |
| `Source\Templates\gcc\startup_stm32l476xx.s` | `Drivers\CMSIS\Device\ST\STM32L4xx\Source\Templates\gcc\` |

---

## SVD file

The SVD file lets the Cortex-Debug extension show peripheral registers by name in the debugger instead of raw hex addresses.

Download `STM32L4x7.svd` from:

https://github.com/cmsis-svd/cmsis-svd-data

Navigate to `data/STMicro/` and find `STM32L4x7.svd`. Click Raw, then save the file as:
```
STM32L476.svd
```

Place it in the project root.

---

## Troubleshooting

**`arm-none-eabi-gcc` not recognized**
The `bin\` folder inside the toolchain is not on PATH. Make sure you added `C:\tools\arm-gnu-toolchain\bin` (not just `C:\tools\arm-gnu-toolchain\`) and opened a new terminal after editing PATH.

**`ninja` not recognized**
You may have created a new environment variable called `ninja` instead of adding to the existing `Path` variable. Delete that variable and add `C:\tools\ninja` as a new entry inside the existing `Path` variable.

**OpenOCD says `Can't find openocd.cfg`**
You are running the OpenOCD command from inside the `build\` folder. Run it from the project root instead:
```
cd C:\projects\nucleo-l476rg
openocd -f openocd.cfg -c "program build/firmware.elf verify reset exit"
```

**Device Manager shows unknown device instead of ST-Link Debug**
The ST-Link driver (STSW-LINK009) is not installed or did not install correctly. Re-run the installer, then unplug and replug the board.

**Firmware flashes successfully but LED does not blink**
Check that jumper JP6 is closed on the Nucleo board — this connects PA5 to the LD2 LED. Also verify the ST-Link driver is installed correctly so the chip resets properly after flashing.

**HardFault on startup (fast LED blink in debug firmware)**
The linker script is missing the init/fini array sections. Make sure your `STM32L476RGTX_FLASH.ld` includes `.preinit_array`, `.init_array`, and `.fini_array` sections in the FLASH region.