#ifndef _GPIO_H_
#define _GPIO_H_
#include <stdint.h>
#include "list.h"


enum gpio_errno {
    EGPIO_SUCCESS = 0,
    EGPIO_NODEV,
    EGPIO_NOENT,
    EGPIO_NOTSUPRT,
    EGPIO_EINVAL,
};

enum gpio_polarity {
    GPIO_POLARITY_NORMAL = 0,
    GPIO_POLARITY_INVERTED,
};
typedef uint8_t gpio_polarity_t;

enum gpio_bias {
    GPIO_BIAS_DISABLE = 0,
    GPIO_BIAS_AS_IS,
    GPIO_BIAS_PULL_UP,
    GPIO_BIAS_PULL_DOWN,
};
typedef uint8_t gpio_bias_t;

enum gpio_direction {
    GPIO_DIRECTION_OUTPUT= 0,
    GPIO_DIRECTION_INPUT,
};
typedef uint8_t gpio_direction_t;

enum gpio_value {
    GPIO_VALUE_LOW= 0,
    GPIO_VALUE_HIGH,
};
typedef uint8_t gpio_value_t;

enum gpio_irq_type {
    GPIO_IRQ_TYPE_NONE		= 0x0,
	GPIO_IRQ_TYPE_EDGE_RISING	= 0x1,
	GPIO_IRQ_TYPE_EDGE_FALLING	= 0x2,
	GPIO_IRQ_TYPE_EDGE_BOTH	= (GPIO_IRQ_TYPE_EDGE_FALLING | GPIO_IRQ_TYPE_EDGE_RISING),
	GPIO_IRQ_TYPE_LEVEL_HIGH	= 0x4,
	GPIO_IRQ_TYPE_LEVEL_LOW	= 0x8,
};
typedef uint8_t gpio_irq_type_t;

struct gpio_t;
struct gpio_controller_t;

struct gpio_cfg_t {
    gpio_polarity_t polarity;
    gpio_direction_t dir;
    gpio_bias_t bias;
};

struct gpio_t {
    int num;
    struct gpio_cfg_t cfg;
    struct gpiochip_t *parent;
    struct list_head list;
};

struct gpio_irq_t {
    int num;
    struct irqchip_t *parent;
    int (*irq_handler)(struct gpio_t *gpio, int trig);
    void *context; 
    gpio_irq_type_t type;
    struct list_head list;
};

struct gpiochip_t;
struct gpiochip_ops_t {
    int (*set_direction)(struct gpio_t *gpio, gpio_direction_t dir);
    int (*get_direction)(struct gpio_t *gpio);

    int (*set_polarity)(struct gpio_t *gpio, gpio_polarity_t polarity);
    int (*get_polarity)(struct gpio_t *gpio);

    int (*set_bias)(struct gpio_t *gpio, gpio_bias_t bias);
    int (*get_bias)(struct gpio_t *gpio);

    int (*set_value)(struct gpio_t *gpio, int value);
    int (*get_value)(struct gpio_t *gpio);

};

struct irqchip_ops_t {
    int (*disable)(struct gpio_irq_t *irq);
    int (*enable)(struct gpio_irq_t *irq);
    int (*configure)(struct gpio_irq_t *irq);
};

struct irqchip_t {
    struct gpio_controller_t *controller;
    void *context;
    int mask;
    struct irqchip_ops_t *ops;
};

struct gpiochip_t {
    struct gpio_controller_t *controller;
    void *context;

    struct gpiochip_ops_t *ops;
};


struct gpio_controller_t {
    struct gpiochip_t *gpio;
    struct irqchip_t *irq;
};


int gpio_set_direction(struct gpio_t *gpio, gpio_direction_t dir);
int gpio_get_direction(struct gpio_t *gpio);
int gpio_set_polarity(struct gpio_t *gpio, gpio_polarity_t polarity);
int gpio_get_polarity(struct gpio_t *gpio);
int gpio_set_bias(struct gpio_t *gpio, gpio_bias_t bias);
int gpio_get_bias(struct gpio_t *gpio);
int gpio_set_value(struct gpio_t *gpio, int value);
int gpio_get_value(struct gpio_t *gpio);
int gpio_irq_disable(struct gpio_irq_t *irq);
int gpio_irq_enable(struct gpio_irq_t *irq);
int gpio_irq_configure(struct gpio_irq_t *irq);

#define DEFINE_IRQGPIO_CONTROLLER(name, _context, gpio_ops, irq_ops) \
    struct gpio_controller_t name; \
    struct gpiochip_t name##_gpiochip = {\
        .controller = &name, \
        .context = _context, \
        .ops = gpio_ops \
    }; \
    struct irqchip_t name##_irqchip = {\
        .controller = &name, \
        .context = _context, \
        .ops = irq_ops, \
        .mask = 0, \
    }; \
    struct gpio_controller_t name = { \
        .gpio = &name##_gpiochip, \
        .irq = &name##_irqchip, \
    }

#define DEFINE_GPIO_CONTROLLER(name, _context, gpio_ops) \
    extern struct gpio_controller_t name; \
    struct gpiochip_t name##_gpiochip = {\
        .controller = &name, \
        .context = _context, \
        .ops = gpio_ops \
    }; \
    struct gpio_controller_t name = { \
        .gpio = &name##_gpiochip, \
        .irq = NULL, \
    }; \
    extern struct gpio_controller_t *name_##controller

#define IMPORT_GPIO_CONTROLLER(name) extern struct gpio_controller_t *name##controller

#endif