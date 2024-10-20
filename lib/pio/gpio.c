#include "gpio.h"


int gpio_set_direction(struct gpio_t *gpio, gpio_direction_t dir) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->set_direction)
        return -EGPIO_NOTSUPRT;

    return chip->ops->set_direction(gpio, dir);
}


int gpio_get_direction(struct gpio_t *gpio) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->get_direction)
        return -EGPIO_NOTSUPRT;

    return chip->ops->get_direction(gpio);
}

int gpio_set_polarity(struct gpio_t *gpio, gpio_polarity_t polarity) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->set_polarity)
        return -EGPIO_NOTSUPRT;

    return chip->ops->set_polarity(gpio, polarity);
}

int gpio_get_polarity(struct gpio_t *gpio) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->get_polarity)
        return -EGPIO_NOTSUPRT;

    return chip->ops->get_polarity(gpio);
}

int gpio_set_bias(struct gpio_t *gpio, gpio_bias_t bias) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->set_bias)
        return -EGPIO_NOTSUPRT;

    return chip->ops->set_bias(gpio, bias);
}

int gpio_get_bias(struct gpio_t *gpio) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->get_bias)
        return -EGPIO_NOTSUPRT;

    return chip->ops->get_bias(gpio);
}

int gpio_set_value(struct gpio_t *gpio, int value) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->set_value)
        return -EGPIO_NOTSUPRT;

    return chip->ops->set_value(gpio, value);
}

int gpio_get_value(struct gpio_t *gpio) {
    struct gpiochip_t *chip;

    if (!gpio || !gpio->parent)
        return -EGPIO_NODEV;
    
    chip = gpio->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->get_value)
        return -EGPIO_NOTSUPRT;

    return chip->ops->get_value(gpio);
}

int gpio_irq_configure(struct gpio_irq_t *irq) {
    struct irqchip_t *chip;

    if (!irq)
        return -EGPIO_NODEV;
    
    chip = irq->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->configure)
        return -EGPIO_NOTSUPRT;

    return chip->ops->configure(irq);
}


int gpio_irq_enable(struct gpio_irq_t *irq) {
    struct irqchip_t *chip;

    if (!irq)
        return -EGPIO_NODEV;
    
    chip = irq->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->enable)
        return -EGPIO_NOTSUPRT;

    return chip->ops->enable(irq);
}


int gpio_irq_disable(struct gpio_irq_t *irq) {
    struct irqchip_t *chip;

    if (!irq)
        return -EGPIO_NODEV;
    
    chip = irq->parent;
    if (!chip->ops)
        return -EGPIO_NOENT;

    if (!chip->ops->disable)
        return -EGPIO_NOTSUPRT;

    return chip->ops->disable(irq);
}