#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "adc.h"
#include "pio.h"
#include "console.h"
#include "i2c_master.h"
#include "display.h"
#include "sd1306_display.h"


#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)

static volatile uint8_t duty = 120;
#define pwm_init(timer_num, side) \
TCCR##timer_num##side |= _BV(COM##timer_num)


static inline void test_pwm()
{
    pio_enable(B, 1, OUTPUT); // D9
    pio_enable(B, 2, OUTPUT); // D10

    TCCR1A |= _BV(COM1B1) | _BV(COM1A1) | _BV(WGM01) | _BV(WGM00);
    TCCR1B |= (1<<WGM02) | (1<<CS00);

    OCR1A = OCR1B = 0;
}

#define set_duty_cycle(duty) OCR2A = duty; OCR2B = duty

int clamp(int value)
{
    return value / 4;
}


void pwm_usart_recv(char c) {

    switch (c)
    {
        case '+':
            ++duty;
            break;
        case '-':
            --duty;
            break;
        case '0':
            duty = 0;
            break;
        case '9':
            duty = 255;
            break;
        case 's':
            duty += 10;
            break;
        case 'h':
            duty += 5;
            break;
        case 'r':
            duty -= 5;
            break;
        case 'b':
            duty -= 10;
            break;
    }
    usart_printf("duty: %d\n\r", duty);
    OCR1B = duty;
    OCR1A = 255 - duty;
}


void rf_send(char c)
{
    uint8_t v = 0;
    for (int i = 0; i < 8; ++i)
    {
        v =  c & (1 << i);
        if (v)
            PORTD |= _BV(5);
        else
            PORTD &= ~_BV(5);
    }
}


void rf_recv(char *c)
{
    *c = 0;
    uint8_t v = 0;
    for (int i = 0; i < 8; ++i)
    {
        v =  PORTD & _BV(6); 
        if (v)
            *c |= (1 << i);
        else
            *c &= ~(1 << i);
    }
}


void rf_write(const char *fmt, ...)
{
    static char buffer[64];
    va_list args;
    va_start(args, fmt);
    int size = sizeof(buffer);
    memset(buffer, 0x0, sizeof(buffer));
    size = vsnprintf(buffer, sizeof(buffer), fmt, args);

    for (int i = 0; i < size; ++i)
    {
        rf_send(buffer[i]);
    }
    va_end(args);
}


void rf_read() {
    char c = 0;
    while (1)
    {
        rf_recv(&c);
        if (c != 0)
            usart_printf("recv: %c\n\r", c);
    }
}

ISR(BADISR_vect)
{
    pio_set(B, 5);
    sei();
}

/*
ISR(PCINT0_vect)
{
    pio_set(D, 5);
    usart_printf("PCINT0 Hello world\n\r");
    sei();
}

ISR(PCINT1_vect)
{
    pio_set(D, 5);
    usart_printf("PCINT1 Hello world\n\r");
    sei();
}

ISR(PCINT2_vect)
{
    pio_disable_irq(2, 22)
    uint8_t v = pio_get(D, 6);
    pio_set_value(D, 5, v);
    usart_printf("PCINT2 Hello world %d\n\r", v);
    pio_enable_irq(2, 22);
    sei();
}
*/
DEFINE_MONOCHROME_SCREEN(mono_screen, 128, 64);
DEFINE_I2C_DISPLAY(oled_sd1306, 128, 64, &mono_screen, I2C0, 0x3C, 100000, &i2c_display_ops, &sd1306_display_ops);


void i2c_command(char *cmd) {
    char *save_ptr, *p;
    int i, ret = 0;
    uint16_t val = 0, subaddr = 0;
    uint8_t slave = 0, dir = 0;
    struct i2c_packet_t pkt;
    for (i = 0, p = cmd; ; ++i, p = NULL) {
        p = strtok_r(p, " ", &save_ptr);
        if (!p)
            break;

        switch (i) {
            case 1:
                dir = !(strncmp(p, "read", 4)) ? TW_READ : TW_WRITE;
                break;
            case 2:
                slave = strtol(p, NULL, 16);
                break;
            case 3:
                subaddr = strtol(p, NULL, 16);
                break;
        }
    }

    i2c_packet_fill(&pkt, dir, slave, subaddr, sizeof(subaddr), (uint8_t *)&val, sizeof(val));
    
    ret = i2c_master_transaction(I2C0, &pkt);

    usart_printf("op: %d slave: 0x%02x subaddr: 0x%02x (ret: %d) val: 0x%04x\n\r", dir, slave, subaddr, ret, val);
}


uint8_t bat_bitmap[] = {
    0xfe, 0xff,
    0x03, 0xf0,
    0x03, 0xf0,
    0x03, 0xf0,
    0x03, 0xf0,
    0xfe, 0xff,
};

struct ebitmap_t bat = {
    .width = 16,
    .height = 6,
    .bits_per_pixel = 1,
    .pixels = bat_bitmap
};

void display_command(char *cmd) {
    char *save_ptr, *p;
    int i, ops = 0;
    uint8_t val = 0, val2 = 0;
    struct point_t offset;

    for (i = 0, p = cmd; ; ++i, p = NULL) {
        p = strtok_r(p, " ", &save_ptr);
        if (!p)
            break;

        switch (i) {
            case 1:
                if (!strncmp(p, "on", 2)) {
                    display_power_on(&oled_sd1306);
                    usart_printf("disp on\n\r");
                    return;
                }
                else if (!strncmp(p, "off", 3)) {
                    display_power_off(&oled_sd1306);
                    usart_printf("disp off\n\r");
                    return;
                }
                else if (!strncmp(p, "clear", 5)) {
                    display_clear(&oled_sd1306);
                    usart_printf("disp clear\n\r");
                    //screen_console_display(&mono_screen);
                    return;
                }
                else if (!strncmp(p, "bright", 6)) {
                    ops = 3;
                    continue;
                }
                else if (!strncmp(p, "hflip", 5)) {
                    ops = 0;
                    continue;
                }
                else if (!strncmp(p, "vflip", 5)) {
                    ops = 1;
                    continue;
                }
                else if (!strncmp(p, "invert", 6)) {
                    ops = 2;
                    continue;
                }
                else if (!strncmp(p, "pix", 6)) {
                    ops = 4;
                    continue;
                }
                else if (!strncmp(p, "draw", 4)) {
                    offset.x = mono_screen.width - bat.width - 1;
                    offset.y = 0;
                    screen_draw_bitmap(&mono_screen, &bat, &offset);
                    display_draw_screen(&oled_sd1306);
                    return;
                }
                else if (!strncmp(p, "print", 5)) {
                    screen_console_display(&mono_screen);
                    return;
                }
                break;
            case 2:
                val = strtol(p, NULL, 10);
                break;
            case 3:
                val2 = strtol(p, NULL, 10);
                break;
        }
    }

    switch (ops)
    {
    case 0:
        display_flip_horizontal(&oled_sd1306, val == 1);
        usart_printf("disp hflip %d\n\r", val == 1);
        break;
    case 1:
        display_flip_vertical(&oled_sd1306, val == 1);
        usart_printf("disp vflip %d\n\r", val == 1);
        break;
    case 2:
        display_invert(&oled_sd1306, val == 1);
        usart_printf("disp invert %d\n\r", val == 1);
        break;
    case 3:
        display_set_brightness(&oled_sd1306, val);
        usart_printf("disp set bright to %d\n\r", val);
        break;
    case 4:
        offset.x = val;
        offset.y = val2;

        screen_set_pixel(&mono_screen, &offset, !screen_get_pixel(&mono_screen, &offset));
        display_draw_screen(&oled_sd1306);
        break;
    
    default:
        break;
    }
}

void on_console_command(char *cmd) {
    

    usart_printf("\n\r");
    display_command(cmd);
    
}





int main() {
    console_init(on_console_command);
    display_init(&oled_sd1306);

    sei();

    /*test_pwm();

    adc_setup(ADC_AREF_PIN, ADC_PRESCALER_128);
    while (1)
    {
        p = adc_read_sync(ADC_CHANNEL_0_PC0);
        if (p != val)
        {
            val = p;
        }
        //if (OCR2A != duty)
        //{
        //    duty = max(120, min(duty, 149));
        //    OCR2A = OCR2B = duty;
        //}

        //rf_read();
        
        //rf_write("hello world!\n\r");
    }*/


  

    while(1) {}

    return 0;
}