# ============================================================
#  Toolchain: arm-none-eabi-gcc для XMC4xxx (Cortex-M4)
# ============================================================

# CMake не пытается искать системные библиотеки целевой платформы
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Тип цели тестовой сборки.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Компиляторы
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Вспомогательные утилиты
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

# OpenOCD
set(CMAKE_OPENOCD openocd)

# GDB
set(CMAKE_GDB arm-none-eabi-gdb)

# Флаги CPU: Cortex-M4, Thumb, hard-float, little-endian
set(TOOLCHAIN_CPU_FLAGS
    "-mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mlittle-endian -munaligned-access"
)

# Флаги компилятора C
set(TOOLCHAIN_C_FLAGS
    "${TOOLCHAIN_CPU_FLAGS} -ffreestanding -fsingle-precision-constant -ffunction-sections -fdata-sections"
)

# Флаги ассемблера
set(TOOLCHAIN_ASM_FLAGS "${TOOLCHAIN_C_FLAGS}")

# Флаги линкера: CPU + удаление мёртвых секций + bare-metal без стандартных библиотек
set(TOOLCHAIN_LINKER_FLAGS
    "-nostartfiles -nodefaultlibs -nostdlib -Wl,--gc-sections -specs=nosys.specs"
)

# Применяем к CMake-механизму инициализации
set(CMAKE_C_FLAGS_INIT          "${TOOLCHAIN_C_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT        "${TOOLCHAIN_ASM_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${TOOLCHAIN_LINKER_FLAGS}")
