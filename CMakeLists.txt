cmake_minimum_required(VERSION 3.20)

# ============================================================
#  Допустимые цели
# ============================================================
set(VALID_TARGETS
    PORT_TVU_V2_0
)

if(NOT DEFINED TARGET OR (NOT "${TARGET}" IN_LIST VALID_TARGETS))
    list(GET VALID_TARGETS 0 FIRST_VALID_TARGET)  # Получаем первый элемент списка
    string(REPLACE ";" "\n  " _valid_list "${VALID_TARGETS}")
    message(FATAL_ERROR
        "TARGET не задан или неверен.\n"
        "Допустимые цели:\n  ${_valid_list}\n"
        "Пример: cmake -DTARGET=${FIRST_VALID_TARGET} -B build"
    )
endif()

# ---- Маппинг: TARGET → PLATFORM + MCU ----
if("${TARGET}" STREQUAL "PORT_TVU_V2_0")
    set(PLATFORM "arm")
    set(MCU "stm32f4xx")
    set(MCU_DEF "STM32F429_439xx")
    set(MCU_LD_SCRIPT "STM32F429IGTx_FLASH.ld")
endif()

# ---- Toolchain file (до project!) ----
if("${PLATFORM}" STREQUAL "arm" AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-arm.cmake")
endif()

# ---- project() ----
if(DEFINED MCU)
    project(app_control C ASM)
else()
    project(app_control C)
endif()

# Имя выходного бинарника
set(PROJECT_BINARY ${PROJECT_NAME}.elf)

# ---- Build type ----
if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "CMAKE_BUILD_TYPE не задан, по-умолчанию используется Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Debug | Release" FORCE)
endif()

# ============================================================
#  ИСХОДНИКИ
# ============================================================

# --- Собственные (единый список, #ifdef PORT_... внутри) ---
# Исходники в основной директории
set(MAIN_SOURCES
    main.c
    modules/modules.c
    lib/fatfs/ff.c
    lib/fatfs/ffsystem.c
    lib/fatfs/ffunicode.c
)
# Список поддиректорий основных исходников
set(MAIN_SOURCES_DIRS
    dma
    eth
    exti
    fmc
    gpio
    graphics
    interrupts
    modbus
    modules/analog
    modules/cli
    modules/digital
    modules/led
    modules/modbus_to_can
    modules/msdi
    modules/mso
    modules/rtc
    modules/settings
    modules/sys
    modules/temp
    lib/bits
    lib/buffer
    lib/crc
    lib/defs
    lib/errors
    lib/fsm
    lib/future
    lib/ini
    lib/iqmath
    lib/net
    lib/newlib
    lib/parity
    lib/rootfs
    lib/utils
    rcc
    reg
    reg_mc
    resampler
    sdcard
    sdio
    spi
    tft9341
    uart
)
# Собираем все .c файлы из разрешённых директорий
foreach(DIR ${MAIN_SOURCES_DIRS})
    file(GLOB_RECURSE TEMP_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_LIST_DIR}/${DIR}/*.c"
    )
    list(APPEND MAIN_SOURCES ${TEMP_SOURCES})
endforeach()

# --- CANopenNode ---
set(CANOPEN_SOURCES
    can/CANopenNode/CANopen.c
    can/CANopenNode/301/CO_fifo.c
    can/CANopenNode/301/CO_PDO.c
    can/CANopenNode/301/CO_HBconsumer.c
    can/CANopenNode/301/crc16-ccitt.c
    can/CANopenNode/301/CO_Node_Guarding.c
    can/CANopenNode/301/CO_NMT_Heartbeat.c
    can/CANopenNode/301/CO_ODinterface.c
    can/CANopenNode/301/CO_SDOclient.c
    can/CANopenNode/301/CO_SDOserver.c
    can/CANopenNode/301/CO_TIME.c
    can/CANopenNode/301/CO_Emergency.c
    can/CANopenNode/301/CO_SYNC.c
    can/CANopenNode/305/CO_LSSslave.c
    can/CANopenNode/305/CO_LSSmaster.c
    can/CANopenNode/303/CO_LEDs.c
    can/CANopenNode/storage/CO_storageEeprom.c
    can/CANopenNode/storage/CO_storage.c
)

# --- CAN bus ---
file(GLOB CAN_BUS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/can/bus/*.c")

# --- CAN ---
set(CAN_SOURCES
    can/CO_CLI_driver.c
    can/CO_data.c
    can/CO_driver.c
    can/OD.c
    can/init/can_init.c
    can/timer/can_timer.c
)

# ============================================================
#  Сборка
# ============================================================

# генерацмя команд компиляции
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_BINARY} 
    ${MAIN_SOURCES}
    ${CANOPEN_SOURCES}
    ${CAN_BUS_SOURCES}
    ${CAN_SOURCES}
)

# ============================================================
#  Общее (для всех платформ)
# ============================================================

set(CMAKE_C_STANDARD 17)

target_compile_options(${PROJECT_BINARY} PRIVATE -fsigned-char -Wall -Wextra -fmessage-length=0)

target_compile_definitions(${PROJECT_BINARY} PRIVATE
    ${TARGET}
    USE_GRAPHICS_FORMAT_RGB_565
    USE_GRAPHICS_FORMAT_BW_1_V
    USE_GRAPHICS_VIRTUAL_BUFFER
)

target_include_directories(${PROJECT_BINARY} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    lib
    can
    can/CANopenNode
    fmc/nand
    fmc/yaffs2
)

# ============================================================
#  Специфичное (для платформ)
# ============================================================

# ---- ARM ----
if("${PLATFORM}" STREQUAL "arm")

    # либо по месту:
    # $<UPPER_CASE:${input_string}>
    # $<STRING:TOUPPER,${input_string}>
    string(TOLOWER "${MCU_DEF}" MCU_DEF_LOWER)

    set(CMSIS_SOURCE_DIR
        "${CMAKE_CURRENT_LIST_DIR}/lib/${MCU}")
    set(CMSIS_TOOLCHAIN_DIR "${CMSIS_SOURCE_DIR}")

    target_sources(${PROJECT_BINARY} PRIVATE
        "${CMSIS_SOURCE_DIR}/system_${MCU}.c"
        "${CMSIS_TOOLCHAIN_DIR}/startup_${MCU_DEF_LOWER}.S"
    )

    set(CMAKE_LINK_DIRS
        "${CMSIS_TOOLCHAIN_DIR}"
    )

    find_file(LINKER_SCRIPT NAMES ${MCU_LD_SCRIPT} PATHS ${CMAKE_LINK_DIRS} NO_DEFAULT_PATH) #REQUIRED

    if(NOT LINKER_SCRIPT)
        message(FATAL_ERROR "Скрипт линкера ${MCU_LD_SCRIPT} не найден в: ${CMAKE_LINK_DIRS}")
    endif()

    set(LINKER_SCRIPT_PATH "${LINKER_SCRIPT}")

    # Compile options
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_C_FLAGS_RELEASE "-Os")
        set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    else()
        set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
        set(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    endif()

    # Defines
    target_compile_definitions(${PROJECT_BINARY} PRIVATE
        ${MCU_DEF}
    )

    # Include paths
    target_include_directories(${PROJECT_BINARY} PRIVATE
        lib/${MCU}
    )

    # Linker
    target_link_options(${PROJECT_BINARY} PRIVATE
        -T${LINKER_SCRIPT_PATH}
        "-Wl,-Map=${PROJECT_BINARY}.map,--cref"
    )

    # Post-build: .bin
    add_custom_target(bin ALL
        DEPENDS ${PROJECT_BINARY}
        COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${PROJECT_BINARY}> ${PROJECT_NAME}.bin
        COMMENT "Generating .bin"
    )

    # Post-build: size
    add_custom_target(size ALL
        DEPENDS ${PROJECT_BINARY}
        COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_BINARY}>
        COMMENT "Printing size"
    )

    # Прошивка
    # Локальная

    # OpenOCD
    # Инициализация
    set(OPENOCD_INIT_COMMANDS
        -f "interface/stlink.cfg"
        -c "transport select swd"
        -f "target/stm32f4x.cfg"
        -c "adapter speed 8000"
        -c "init"
        -c "gdb breakpoint_override hard"
    )

    # Прошивка
    add_custom_target(openocd-flash
        COMMAND ${CMAKE_OPENOCD}
            ${OPENOCD_INIT_COMMANDS}
            -c "reset halt"
            -c "program $<TARGET_FILE:${PROJECT_BINARY}> verify reset exit"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Flashing firmware via OpenOCD (program)"
        DEPENDS ${PROJECT_BINARY}
    )

    # Сервер отладки
    add_custom_target(openocd-gdbserver
        COMMAND ${CMAKE_OPENOCD}
            ${OPENOCD_INIT_COMMANDS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Starting OpenOCD GDB server"
        DEPENDS ${PROJECT_BINARY}
    )
    
    # Удалённая - сервер отладки

    # OpenOCD
    # Host
    set(OPENOCD_REMOTE_HOST "rpi-debug")
    # Порт TCL
    set(OPENOCD_TCL_PORT 6667)
    # Порт Telnet
    set(OPENOCD_TELNET_PORT 4445)
    # Порт GDB
    set(OPENOCD_GDB_PORT 3334)

    # Прошивка через GDB (файл остаётся локально)
    add_custom_target(remote-flash-gdb
        COMMAND ${CMAKE_GDB} -batch
            -ex "target remote ${OPENOCD_REMOTE_HOST}:${OPENOCD_GDB_PORT}"
            -ex "monitor reset halt"
            -ex "load"
            -ex "monitor reset run"
            -ex "monitor resume"
            -ex "monitor exit"
            -ex "detach"
            $<TARGET_FILE:${PROJECT_BINARY}>
        COMMENT "Flashing firmware to remote device via GDB (${OPENOCD_REMOTE_HOST}:${OPENOCD_GDB_PORT})"
        DEPENDS ${PROJECT_BINARY}
    )

endif()
