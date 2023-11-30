set(AVR_PROJECT_NAME "avr")
set(AVR_PROJECT_VERSION 1.0.0)
set(AVR_PROJECT_DEVICE "atmega328p")
set(AVR_F_CPU_CLK_SPEED_HZ 16000000UL)
set(SERIAL_CONSOLE_BAUDRATE 38400)

set(AVR_PROJECT_SOURCES
    src/main.c
    lib/usart/usart.h
    lib/usart/usart.c
    lib/pio/pio.h
    lib/adc/adc.h
    src/VWutil/crc16.h
    src/VirtualWire_Config.h
    src/VirtualWire.h
    src/VirtualWire.cpp
)

set(AVR_PROJECT_HEADERS_DIR
    lib/usart/
    lib/pio/
    lib/adc/
    src
)

set(AVR_EXTRA_DEFINITIONS
    -DVW_PLATFORM=VW_PLATFORM_GENERIC_AVR8
    -DVW_TX_DDR=DDRD
    -DVW_RX_DDR=DDRD
    -DVW_RX_PORT=PORTD
    -DVW_TX_PORT=PORTD
    -DVW_TX_PIN=5
    -DVW_RX_PIN=6
    -DVW_TIMER_INDEX=2
)