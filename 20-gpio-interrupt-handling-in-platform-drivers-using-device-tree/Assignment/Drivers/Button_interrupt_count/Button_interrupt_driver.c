#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Button interrupt driver");

static int press_count = 0;

static irqreturn_t button_isr(int irq, void *dev_id)
{
    press_count++;
    pr_info("Button press count: %d\n", press_count);
    return IRQ_HANDLED;
}

static int __init button_init(void)
{
    int irq = gpio_to_irq(18);
    int ret = request_irq(irq, button_isr, IRQF_TRIGGER_RISING,"Button handler", NULL);
    if (ret)
        pr_err("Failed to request IRQ\n");
    return ret;
}

static void __exit button_exit(void)
{
    int irq = gpio_to_irq(18);
    free_irq(irq, NULL);
}

static int button_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Press count: %d\n", press_count);
    return 0;
}

static int button_proc_open(struct inode *inode, struct file *file)
{
    return single_open(inode, file, button_proc_show);
}

static const struct file_operations button_proc_ops = {
    .owner = THIS_MODULE,
    .open = button_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init button_init_proc(void)
{
    struct proc_dir_entry *proc_entry;
    proc_entry = proc_create("button_count", 0, NULL, &button_proc_ops);
    if (!proc_entry)
        pr_err("Failed to create proc entry\n");
    return 0;
}

static void __exit button_exit_proc(void)
{
    proc_remove("button_count");
}

module_init(button_init);
module_exit(button_exit);

module_init(button_init_proc);
module_exit(button_exit_proc);
