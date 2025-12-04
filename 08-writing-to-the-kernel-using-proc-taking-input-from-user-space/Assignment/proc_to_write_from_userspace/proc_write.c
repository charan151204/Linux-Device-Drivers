#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define PROC_NAME "writer"
#define MAX_LEN 100

static int logging_enabled = 0;   // Flag to control logging
static char message[MAX_LEN];     // last user message (optional)

// READ — Show ON or OFF status
ssize_t writer_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char status[32];

    if (logging_enabled)
        sprintf(status, "Logging: ON\n");
    else
        sprintf(status, "Logging: OFF\n");

    return simple_read_from_buffer(buf, count, ppos, status, strlen(status));
}

// WRITE — Accept ON or OFF
ssize_t writer_write(struct file *file, const char __user *buf,
                     size_t count, loff_t *ppos)
{
    if (count > MAX_LEN - 1)
        count = MAX_LEN - 1;

    // Copy from userspace
    if (copy_from_user(message, buf, count))
        return -EFAULT;

    message[count] = '\0';   // null-terminate

    // Compare input
    if (strncmp(message, "ON", 2) == 0) {
        logging_enabled = 1;
        printk(KERN_INFO "Logging ENABLED\n");
    }
    else if (strncmp(message, "OFF", 3) == 0) {
        logging_enabled = 0;
        printk(KERN_INFO "Logging DISABLED\n");
    }
    else {
        printk(KERN_INFO "Invalid input: %s\n", message);
    }

    return count;
}

static const struct proc_ops writer_fops = {
    .proc_read  = writer_read,
    .proc_write = writer_write,
};

static int __init proc_writer_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &writer_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit proc_writer_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_writer_init);
module_exit(proc_writer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Simple ON/OFF flag using /proc");
