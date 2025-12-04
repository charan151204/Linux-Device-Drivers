#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUFFER_SIZE 1024

static int major;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;
static int last_bytes_written = 0;

static int dev_open(struct inode *inode, struct file *file)
{
    open_count++;
    printk(KERN_INFO "[%s] Device opened %d times\n", DEVICE_NAME, open_count);
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf,size_t len, loff_t *offset)
{
    char msg[64];
    int msg_len;

    msg_len = snprintf(msg, sizeof(msg),"You wrote %d bytes last time.\n", last_bytes_written);

    return simple_read_from_buffer(buf, len, offset, msg, msg_len);
}

static ssize_t dev_write(struct file *file, const char __user *buf,size_t len, loff_t *offset)
{
    if (len > BUFFER_SIZE - 1)
        len = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, buf, len))
        return -EFAULT;

    device_buffer[len] = '\0';
    last_bytes_written = len;

    printk(KERN_INFO "[%s] Received %d bytes: %s\n", DEVICE_NAME, last_bytes_written, device_buffer);

    return len;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] Device closed\n", DEVICE_NAME);
    return 0;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};

static int __init char_driver_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) 
    {
        printk(KERN_ALERT "Failed to register char device\n");
        return major;
    }

    printk(KERN_INFO "Char driver registered with major number %d\n", major);
    return 0;
}

static void __exit char_driver_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Char driver unregistered\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Character driver with byte counter");
