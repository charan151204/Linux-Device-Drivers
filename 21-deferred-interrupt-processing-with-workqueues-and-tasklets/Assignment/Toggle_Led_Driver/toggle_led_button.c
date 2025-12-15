#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Toggle LED Driver");

static int led_gpiod;
static int btn_gpiod;

static ktime_t last_press_time;

static irqreturn_t btn_isr(int irq, void *dev_id)
{
    struct work_struct *work = dev_id;
    ktime_t now = ktime_get();
    s64 time_diff_ms = ktime_ms_delta(now, last_press_time);
    if (time_diff_ms < 100) {
        return IRQ_HANDLED;
    }
    last_press_time = now;
    queue_work(work, btn_work_handler);
    return IRQ_HANDLED;
}

static void btn_work_handler(struct work_struct *work)
{
    static int led_state = 0;
    led_state = !led_state;
    gpiod_set_value(led_gpiod, led_state);
}

static int __init toggle_led_init(void)
{
    led_gpiod = gpio_request_one("led_gpiod", GPIOF_OUT_INIT_HIGH, "led");
    btn_gpiod = gpio_request_one("btn_gpiod", GPIOF_IN, "btn");
    irq = gpio_to_irq(btn_gpiod);
    request_irq(irq, btn_isr, IRQF_TRIGGER_FALLING, "btn_isr", &btn_isr_work);
    INIT_WORK(&btn_isr_work, btn_work_handler);
    last_press_time = ktime_get();
    return 0;
}

static void __exit toggle_led_exit(void)
{
    free_irq(gpio_to_irq(btn_gpiod), &btn_isr_work);
    gpio_free(btn_gpiod);
    gpio_free(led_gpiod);
}

module_init(toggle_led_init);
module_exit(toggle_led_exit);

MODULE_DESCRIPTION("Toggle LED Driver");
MODULE_AUTHOR("Charan");
MODULE_LICENSE("GPL");
