include(project.cmake)
set(CMAKE_SYSTEM_NAME Generic) # Or name of your OS if you have one
set(CMAKE_SYSTEM_PROCESSOR avr) 
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_LINKER avr-ld)
set(CMAKE_OBJCOPY avr-objcopy)

set(CMAKE_INCLUDE_DIRECTORIES_BEFORE /usr/lib/avr/include)
set(CMAKE_LINK_DIRECTORIES_BEFORE /usr/lib/avr/lib)

add_compile_options(
    -std=gnu11 -mmcu=${AVR_PROJECT_DEVICE} -DF_CPU=${AVR_F_CPU_CLK_SPEED_HZ}
    -ffunction-sections -fdata-sections -ffreestanding -fno-strict-aliasing
    -Os -Wall -fmessage-length=0 -I/usr/lib/avr/include
)

add_link_options(
    -mthumb -mmcu=${AVR_PROJECT_DEVICE} -Xlinker -Map=${AVR_PROJECT_NAME}.map 
    -std=gnu11 -Wl,--start-group -lm  -Wl,--end-group -Wl,--gc-sections -g3 -Os -ggdb 
    -T${CMAKE_CURRENT_BINARY_DIR}/../src/${AVR_PROJECT_LDSCRIPT} 
    -Os -Wl,--cref -Wl,--print-memory-usage
)

include_directories(${AVR_PROJECT_HEADERS_DIR})
if(DEFINED SERIAL_CONSOLE_BAUDRATE)
add_definitions(-DBAUD=${SERIAL_CONSOLE_BAUDRATE})
endif()