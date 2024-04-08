set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_LINKER arm-none-eabi-ld)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy CACHE INTERNAL "")
set(CMAKE_RANLIB arm-none-eabi-ranlib CACHE INTERNAL "")
set(CMAKE_SIZE arm-none-eabi-size CACHE INTERNAL "")
set(CMAKE_STRIP arm-none-eabi-strip CACHE INTERNAL "")

set(CMAKE_C_FLAGS "-Wno-psabi -march=armv6-m -ffreestanding -mthumb -fdata-sections -ffunction-sections" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections --specs=nano.specs" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG "-O1 -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
