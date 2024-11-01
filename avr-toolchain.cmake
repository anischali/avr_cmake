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
    -I/usr/lib/avr/include 
    -DF_CPU=${AVR_F_CPU_CLK_SPEED_HZ} 
    -ggdb -Os -g3 -Wall -c -fmessage-length=0 
    -ffunction-sections -fdata-sections 
    -mmcu=${AVR_PROJECT_DEVICE}
)

add_link_options(
    -mmcu=${AVR_PROJECT_DEVICE} -Xlinker -Map=${AVR_PROJECT_NAME}.map -Xlinker 
    -Wl,--cref -Wl,--print-memory-usage 
)

include_directories(${AVR_PROJECT_HEADERS_DIR})
if(DEFINED SERIAL_CONSOLE_BAUDRATE)
add_definitions(-DBAUD=${SERIAL_CONSOLE_BAUDRATE})
endif()