// ## 2. Rising Edge Support

// ### Modify the driver to:

// Accept a DT property "irq-trigger" (string: "rising", "falling", "both")

// Parse it and choose IRQF_TRIGGER_RISING, etc., dynamically

static int get_irq_trigger(const struct device_node *np)
{
    const char *trigger_str;
    int trigger = 0;

    if (of_property_read_string(np, "irq-trigger", &trigger_str) != 0)
        return 0;

    if (strcmp(trigger_str, "rising") == 0)
        trigger = IRQF_TRIGGER_RISING;
    else if (strcmp(trigger_str, "falling") == 0)
        trigger = IRQF_TRIGGER_FALLING;
    else if (strcmp(trigger_str, "both") == 0)
        trigger = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;

    return trigger;
}


#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("GPIO interrupt handler using device tree");

static int irq;
static struct gpio_desc *gpio;

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    gpio_get_value(gpio);
    return IRQ_HANDLED;
}

static int __init gpio_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    int trigger = get_irq_trigger(np);

    gpio = of_get_gpio(np, 0);
    if (IS_ERR(gpio))
        return PTR_ERR(gpio);

    irq = gpio_to_irq(gpio);
    if (irq < 0)
        return irq;

    return request_irq(irq, gpio_irq_handler, trigger, "gpio-irq", NULL);
}

static int __exit gpio_remove(struct platform_device *pdev)
{
    free_irq(irq);
    return 0;
}

static const struct of_device_id gpio_ids[] = {
    { .compatible = "gpio-irq", },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_ids);

static struct platform_driver gpio_driver = {
    .probe = gpio_probe,
    .remove = gpio_remove,
    .driver = {
        .name = "gpio-irq",
        .of_match_table = gpio_ids,
    },
};

module_platform_driver(gpio_driver);
