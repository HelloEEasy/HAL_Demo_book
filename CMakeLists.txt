cmake_minimum_required(VERSION 3.22)

project(00_Template LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component(PLAYBOOK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(PLATFORM_ROOT "${PLAYBOOK_ROOT}/Platform")

set(HAL_SOURCE_DIR "${PLATFORM_ROOT}/Drivers/STM32F1xx_HAL_Driver/Src")

add_executable(${PROJECT_NAME}
    startup_stm32f103xb.s
    Core/Src/main.c
    Core/Src/gpio.c
    Core/Src/stm32f1xx_hal_msp.c
    Core/Src/stm32f1xx_it.c
    Core/Src/system_stm32f1xx.c
    App/Src/app.c
    ${PLATFORM_ROOT}/GCC/syscalls.c
    ${PLATFORM_ROOT}/GCC/sysmem.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_cortex.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_exti.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_flash.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_flash_ex.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_gpio.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_gpio_ex.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_pwr.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_rcc.c
    ${HAL_SOURCE_DIR}/stm32f1xx_hal_rcc_ex.c
)

# 显式保留 .elf 后缀，使 VSCode/调试工具无需猜测无后缀裸机文件的格式。
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

target_include_directories(${PROJECT_NAME} PRIVATE
    Core/Inc
    App/Inc
    BSP/Inc
    Port/Inc
    ${PLATFORM_ROOT}/Drivers/STM32F1xx_HAL_Driver/Inc
    ${PLATFORM_ROOT}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    ${PLATFORM_ROOT}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ${PLATFORM_ROOT}/Drivers/CMSIS/Include
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    USE_HAL_DRIVER
    STM32F103xB
    $<$<CONFIG:Debug>:DEBUG>
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    $<$<CONFIG:Debug>:-O0;-g3>
    $<$<CONFIG:Release>:-Os;-g0>
    $<$<COMPILE_LANGUAGE:ASM>:-x;assembler-with-cpp>
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103XX_FLASH.ld
    --specs=nano.specs
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
)

target_link_libraries(${PROJECT_NAME} PRIVATE m)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "生成 HEX/BIN 固件并统计占用空间"
)
