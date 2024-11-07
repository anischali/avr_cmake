set(AVR_PROJECT_NAME "avr")
set(AVR_PROJECT_VERSION 1.0.0)
set(AVR_PROJECT_DEVICE "atmega328p")
set(AVR_F_CPU_CLK_SPEED_HZ 16000000UL)
set(SERIAL_CONSOLE_BAUDRATE 38400)

set(AVR_PROJECT_SOURCES
    src/main.c
    lib/usart/usart.c
    lib/console/console.c
    lib/i2c/i2c_master.c
    lib/display/display.c
    lib/display/screen.c
    lib/display/display_i2c.c
    lib/display/sd1306/sd1306_display.c
    lib/font/font.c
)

set(AVR_PROJECT_HEADERS_DIR
    lib/usart/
    lib/pio/
    lib/adc/
    lib/i2c/
    lib/common/
    lib/console/
    lib/list/
    lib/font/
    lib/display/
    lib/display/sd1306/
)


set(AVR_EXTRA_DEFINITIONS
    -D__AVR_ATmega328P__=1
)