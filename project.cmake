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
)

set(AVR_PROJECT_HEADERS_DIR
    lib/usart/
    lib/pio/
    lib/adc/
)


set(AVR_EXTRA_DEFINITIONS)