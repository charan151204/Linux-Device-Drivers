#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("GPIO button driver with sysfs interface");

#define DEVICE_NAME "gpio_button_sysfs"
#define GPIO_BUTTON 18

static int gpio_button = -1;

/**
 * sysfs_show - Show the value of the GPIO button sysfs file
 * @kobj: The kobject representing the sysfs file
 * @attr: The kobj_attribute representing the sysfs file
 * @buf: The buffer to store the value in
 *
 * This function gets the current value of the GPIO button and
 * stores it in the given buffer as a string representation of
 * an integer followed by a newline character.
 *
 * Returns the number of bytes written to the buffer on success.
 */

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int value;
    gpio_get_value_cansleep(gpio_button, &value);
    return sprintf(buf, "%d\n", value);
}

/**
 * sysfs_store - Store a value in the GPIO button sysfs file
 * @kobj: The kobject representing the sysfs file
 * @attr: The kobj_attribute representing the sysfs file
 * @buf: The buffer containing the value to store
 * @count: The size of the buffer
 *
 * This function takes a string representation of a value from the
 * given buffer and stores it in the GPIO button. The value is
 * expected to be a single integer.
 *
 * Returns the number of bytes written to the sysfs file on success,
 * -EINVAL if the buffer does not contain a valid integer value.
 */
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int value;
    if (sscanf(buf, "%d", &value) != 1)
        return -EINVAL;
    gpio_set_value_cansleep(gpio_button, value);
    return count;
}

static struct sysfs_ops sysfs_gpio_button_ops = {
    .show = sysfs_show,
    .store = sysfs_store,
};

static struct kobj_attribute gpio_button_attribute = {
    .attr = {
        .name = "gpio_button",
        .mode = 0660,
    },
    .show = sysfs_show,
    .store = sysfs_store,
};

/**
 * gpio_button_sysfs_init - Initialize the GPIO button sysfs driver
 *
 * This function requests the GPIO button pin and creates a sysfs file
 * for it. The sysfs file allows userspace to read and write the state
 * of the GPIO button.
 *
 * Return: 0 on success, -ENODEV on failure to request the GPIO pin.
 */
static int __init gpio_button_sysfs_init(void)
{
    gpio_button = gpio_request_one(GPIO_BUTTON, GPIOF_IN, "gpio_button");
    if (gpio_button < 0)
        return -ENODEV;
    sysfs_create_file(kernel_kobj, &gpio_button_attribute);
    return 0;
}

static void __exit gpio_button_sysfs_exit(void)
{
    gpio_free(gpio_button);
    sysfs_remove_file(kernel_kobj, &gpio_button_attribute);
}

module_init(gpio_button_sysfs_init);
module_exit(gpio_button_sysfs_exit);
