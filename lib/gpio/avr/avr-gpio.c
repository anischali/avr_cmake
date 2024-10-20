#include "gpio.h"
#include <avr/io.h>
#include "common.h"

struct pio_port_t {
    uint8_t min_pin;
    uint8_t max_pin;
    __IO uint8_t ddr;
    __IO uint8_t port;
};

#define AVR_PORT_IO(_port, i, min, max) \
    { .index = i, min_pin = min, .max_pin = max, .ddr = DDR##_port, .port = PORT##_port }

#define MAX_PORT_NUM 4

struct pio_port_t const ports_regs[MAX_PORT_NUM] = {
    AVR_PORT_IO(A, 0, 0, 8),
    AVR_PORT_IO(B, 1, 0, 8),
    AVR_PORT_IO(C, 2, 0, 8),
    AVR_PORT_IO(D, 3, 0, 8),
};

//#define DEFINE_INTERRUPT_PIO(name, port, pin_num, irq_no, irq_grp) \
//    static struct pio_interrupt_t name##_irq = { \
//        .irq_num = PCINT##irq_no, \
//        .pcmsk = PCMSK##irq_grp, \
//        .pcie = PCIE##irq_grp


//#define pio_interrupt(pio)        (PCICR |= (_BV(PCIE##pc_group))); (PCMSK##pc_group |= (_BV(PCINT##num)))

//#define pio_disable_irq(pc_group, num)  (PCMSK##pc_group &= ~(_BV(PCINT##num)));
//#define pio_enable_irq(pc_group, num)   (PCMSK##pc_group |= (_BV(PCINT##num)));



/*
#define DEFINE_INTERRUPT_PIO(name, port, pin_num, irq_no, irq_grp) \
    static struct pio_interrupt_t name##_irq = { \
        .irq_num = PCINT##irq_no, \
        .pcmsk = PCMSK##irq_grp, \
        .pcie = PCIE##irq_grp, \
    }, \
    static struct pio_t name = { \
        .ddr = DDR##port, \
        .port = PORT##port, \
        .pin = pin_num, \
        .dir = _dir, \
        .irq_ctx = &name##_irq, \
    }

*/

int pio_set_direction(struct gpio_t *gpio, gpio_direction_t dir) {
    uint8_t port = gpio->num & 0xf0;
    uint8_t pin = gpio->num & 0x0f;
    struct pio_port_t *regs;

    __assert(port >= 0 && port < MAX_PORT_NUM, EGPIO_NOENT);
    __assert(pin >= ports_regs[port].min_pin && pin < ports_regs[port].max_pin, EGPIO_NOENT);

    regs = &ports_regs[port];

    if (gpio->cfg.dir == GPIO_DIRECTION_OUTPUT) {
        regs->ddr |= BIT(pin);
    }
    else {
        regs->ddr &= ~BIT(pin);
    }

    return 0;
}


int pio_get_direction(struct gpio_t *gpio) {
    uint8_t port = gpio->num & 0xf0;
    uint8_t pin = gpio->num & 0x0f;
    struct pio_port_t *regs;

    __assert(port >= 0 && port < MAX_PORT_NUM, EGPIO_NOENT);
    __assert(pin >= ports_regs[port].min_pin && pin < ports_regs[port].max_pin, EGPIO_NOENT);

    regs = &ports_regs[port];

    if (regs->ddr & BIT(pin))
        return GPIO_DIRECTION_OUTPUT;
    else
        return GPIO_DIRECTION_INPUT;
}


int pio_set_bias(struct gpio_t *gpio, gpio_bias_t bias) {
    uint8_t port = gpio->num & 0xf0;
    uint8_t pin = gpio->num & 0x0f;
    struct pio_port_t *regs;

    __assert(port >= 0 && port < MAX_PORT_NUM, EGPIO_NOENT);
    __assert(pin >= ports_regs[port].min_pin && pin < ports_regs[port].max_pin, EGPIO_NOENT);

    regs = &ports_regs[port];

    if (gpio->cfg.dir == GPIO_DIRECTION_INPUT) {
        if (bias == GPIO_BIAS_PULL_UP)
            regs->port |= BIT(pin);
        
        else if (bias == GPIO_BIAS_DISABLE)
            regs->port &= ~BIT(pin);
    }

    return 0;
}


int pio_get_bias(struct gpio_t *gpio) {
    uint8_t port = gpio->num & 0xf0;
    uint8_t pin = gpio->num & 0x0f;
    struct pio_port_t *regs;

    __assert(port >= 0 && port < MAX_PORT_NUM, EGPIO_NOENT);
    __assert(pin >= ports_regs[port].min_pin && pin < ports_regs[port].max_pin, EGPIO_NOENT);

    regs = &ports_regs[port];

    if (gpio->cfg.dir == GPIO_DIRECTION_INPUT && regs->port & BIT(pin))
        return GPIO_BIAS_PULL_UP;

    return GPIO_BIAS_DISABLE;
}


int pio_set_value(struct gpio_t *gpio, int value) {
    uint8_t port = gpio->num & 0xf0;
    uint8_t pin = gpio->num & 0x0f;
    struct pio_port_t *regs;

    __assert(port >= 0 && port < MAX_PORT_NUM, EGPIO_NOENT);
    __assert(pin >= ports_regs[port].min_pin && pin < ports_regs[port].max_pin, EGPIO_NOENT);

    regs = &ports_regs[port];

    if (gpio->cfg.dir == GPIO_DIRECTION_OUTPUT) {
        if (value)
            regs->port |= BIT(pin);
        else
            regs->port &= ~BIT(pin);
    }

    return 0;
}

int pio_get_value(struct gpio_t *gpio) {
    uint8_t port = gpio->num & 0xf0;
    uint8_t pin = gpio->num & 0x0f;
    struct pio_port_t *regs;

    __assert(port >= 0 && port < MAX_PORT_NUM, EGPIO_NOENT);
    __assert(pin >= ports_regs[port].min_pin && pin < ports_regs[port].max_pin, EGPIO_NOENT);

    regs = &ports_regs[port];

    return (regs->port & BIT(pin) != 0);
}

static struct gpiochip_ops_t avr_gpio_ops = {
    .get_direction = pio_get_direction,
    .set_direction = pio_set_direction,
    .get_bias = pio_get_bias,
    .set_bias = pio_set_bias,
    .get_value = pio_get_value,
    .set_value = pio_set_value,
};


DEFINE_GPIO_CONTROLLER(avr_gpio, NULL, &avr_gpio_ops);