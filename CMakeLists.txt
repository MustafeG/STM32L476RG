cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi-gcc.cmake)

project(nucleo-l476rg C ASM)

set(CMAKE_C_STANDARD 11)

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

set(COMPILE_FLAGS
    ${MCU_FLAGS}
    -Wall
    -O0
    -g3
    -DSTM32L476xx
    -ffunction-sections
    -fdata-sections
)

set(LINK_FLAGS
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/STM32L476RGTX_FLASH.ld
    -Wl,--gc-sections
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
)

add_compile_options(${COMPILE_FLAGS})
add_link_options(${LINK_FLAGS})

include_directories(
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32L4xx/Include
    Inc
)

add_executable(firmware.elf
    Src/main.c
    Src/gpio.c
    Src/systick.c
    # Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
    Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l476xx.s
)

add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
    COMMAND ${CMAKE_SIZE} firmware.elf
)