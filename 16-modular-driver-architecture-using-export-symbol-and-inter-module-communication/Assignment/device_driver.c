#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Device driver using core module functions");

#define BUF_LEN 256
static char buffer[BUF_LEN];

/* Declare external functions from core module */
extern void core_log_message(const char *msg);
extern const char *core_version(void);

static ssize_t dev_write(struct file *file, const char __user *buf, 
                         size_t len, loff_t *off)
{
    if (len >= BUF_LEN) 
        len = BUF_LEN - 1;

    if (copy_from_user(buffer, buf, len))
        return -EFAULT;

    buffer[len] = '\0';
    core_log_message(buffer);  /* Call function from core module */
    
    return len;
}

static int dev_open(struct inode *inode, struct file *file)
{
    const char *version = core_version();
    printk(KERN_INFO "[device_driver] using core version: %s\n", version);
    return 0;
}

static int dev_release(struct inode *inode, struct file *file) 
{ 
    return 0; 
}

/* File operations structure */
static struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .write = dev_write,
    .release = dev_release,
};

static int __init device_init(void)
{
    printk(KERN_INFO "device_driver loaded\n");
    /* Normally you'd register a device here */
    return 0;
}

static void __exit device_exit(void)
{
    printk(KERN_INFO "device_driver unloaded\n");
}

module_init(device_init);
module_exit(device_exit);