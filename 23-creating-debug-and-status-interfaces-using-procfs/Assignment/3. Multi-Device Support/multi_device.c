#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>

#define PROC_NAME "gpio"
#define BTN0_IRQ 12
#define BTN1_IRQ 13

static int read_counter0 = 0;
static int read_counter1 = 0;
static int led_state0 = 0;
static int led_state1 = 0;

static irqreturn_t btn0_isr(int irq, void *dev_id)
{
    read_counter0++;
    led_state0 = !led_state0;
    return IRQ_HANDLED;
}

static irqreturn_t btn1_isr(int irq, void *dev_id)
{
    read_counter1++;
    led_state1 = !led_state1;
    return IRQ_HANDLED;
}

static int proc_gpio_read(struct seq_file *m, void *v)
{
    seq_printf(m, "BTN0 IRQ: %d\n", BTN0_IRQ);
    seq_printf(m, "BTN0 Count: %d\n", read_counter0);
    seq_printf(m, "BTN0 LED State: %d\n", led_state0);
    seq_printf(m, "BTN1 IRQ: %d\n", BTN1_IRQ);
    seq_printf(m, "BTN1 Count: %d\n", read_counter1);
    seq_printf(m, "BTN1 LED State: %d\n", led_state1);
    return 0;
}

static int proc_gpio_open(struct inode *inode, struct file *file)
{
    return single_open(inode, file, proc_gpio_read);
}

static const struct file_operations proc_gpio_ops = {
    .owner = THIS_MODULE,
    .open = proc_gpio_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init proc_gpio_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &proc_gpio_ops);
    request_irq(BTN0_IRQ, btn0_isr, 0, "btn0", NULL);
    request_irq(BTN1_IRQ, btn1_isr, 0, "btn1", NULL);
    return 0;
}

static void __exit proc_gpio_exit(void)
{
    remove_proc_entry(PROC_NAME);
    free_irq(BTN0_IRQ);
    free_irq(BTN1_IRQ);
}

module_init(proc_gpio_init);
module_exit(proc_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Multi-device support using procfs");
