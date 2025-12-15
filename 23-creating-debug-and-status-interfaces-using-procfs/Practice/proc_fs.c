#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static struct proc_dir_entry *proc_entry;

static int gpiobtn_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Driver: GPIO Button + LED\n");
    seq_printf(m, "IRQ Number: %d\n", irq_num);
    seq_printf(m, "Press Count: %d\n", press_count);
    seq_printf(m, "LED State: %s\n", led_state ? "ON" : "OFF");
    return 0;
}

static int gpiobtn_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, gpiobtn_proc_show, NULL);
}

static const struct proc_ops gpiobtn_proc_ops = {
    .proc_open    = gpiobtn_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init gpio_button_sysfs_init(void)
{
    gpio_button = gpio_request_one(GPIO_BUTTON, GPIOF_IN, "gpio_button");
    if (gpio_button < 0)
        return -ENODEV;
    sysfs_create_file(kernel_kobj, &gpio_button_attribute);
    proc_entry = proc_create("gpiobtn_info", 0, NULL, &gpiobtn_proc_ops);
    if (!proc_entry)
        dev_warn(dev, "Failed to create /proc/gpiobtn_info\n");
    return 0;
}

static void __exit gpio_button_sysfs_exit(void)
{
    gpio_free(gpio_button);
    sysfs_remove_file(kernel_kobj, &gpio_button_attribute);
    proc_remove(proc_entry);
}

module_init(gpio_button_sysfs_init);
module_exit(gpio_button_sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("GPIO button sysfs driver");