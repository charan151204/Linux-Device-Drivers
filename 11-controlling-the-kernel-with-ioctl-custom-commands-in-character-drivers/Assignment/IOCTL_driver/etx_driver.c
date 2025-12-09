#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define MY_IOC_MAGIC 'k'
#define IOCTL_SET_DEBUG _IOW(MY_IOC_MAGIC, 3, int)

static dev_t dev;
static struct cdev etx_cdev;

static int debug_mode = 0;

static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "etx_driver: Device opened\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "etx_driver: Device closed\n");
    return 0;
}

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int value;

    switch (cmd) {
        case IOCTL_SET_DEBUG:
            if (copy_from_user(&value, (int __user *)arg, sizeof(int)))
                return -EFAULT;

            debug_mode = value;
            printk(KERN_INFO "etx_driver: Debug mode = %d\n", debug_mode);
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

static ssize_t etx_write(struct file *file, const char __user *buf,
                         size_t len, loff_t *off)
{
    char data[100];

    if (len >= sizeof(data))
        len = sizeof(data) - 1;

    if (copy_from_user(data, buf, len))
        return -EFAULT;

    data[len] = '\0';

    if (debug_mode == 1) {
        printk(KERN_INFO "etx_driver(write): Received %zu bytes\n", len);
        printk(KERN_INFO "etx_driver(write): Data = %s\n", data);
    }

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = etx_open,
    .release = etx_release,
    .unlocked_ioctl = etx_ioctl,
    .write = etx_write
};

static int __init etx_init(void)
{
    alloc_chrdev_region(&dev, 0, 1, "etx_driver");
    cdev_init(&etx_cdev, &fops);
    cdev_add(&etx_cdev, dev, 1);

    printk(KERN_INFO "etx_driver loaded. Major=%d Minor=%d\n",
           MAJOR(dev), MINOR(dev));
    return 0;
}

static void __exit etx_exit(void)
{
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "etx_driver unloaded\n");
}

module_init(etx_init);
module_exit(etx_exit);

MODULE_LICENSE("GPL");


// sudo mknod /dev/etx_driver c 240 0
// sudo chmod 666 /dev/etx_driver
