#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define MY_IOC_MAGIC 'k'
#define IOCTL_SET_DEBUG _IOW(MY_IOC_MAGIC, 3, int)

static dev_t devnum;
static struct cdev etx_cdev;
static struct class *dev_class;
static struct device *dev_device;

static int debug_mode = 0;

/* -------- Module Parameters -------- */

static char *device_name = "autodev";
static char *class_name  = "myclass";

module_param(device_name, charp, 0660);
module_param(class_name,  charp, 0660);

MODULE_PARM_DESC(device_name, "Device name created in /dev/");
MODULE_PARM_DESC(class_name,  "Class name under /sys/class/");

/* ---------------------------------- */

static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s: Device opened\n", device_name);
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s: Device closed\n", device_name);
    return 0;
}

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int value;

    switch(cmd) {
        case IOCTL_SET_DEBUG:
            if (copy_from_user(&value, (int __user *)arg, sizeof(int)))
                return -EFAULT;

            debug_mode = value;
            printk(KERN_INFO "%s: Debug mode = %d\n", device_name, debug_mode);
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

    if (debug_mode)
        printk(KERN_INFO "%s(write): %s\n", device_name, data);

    return len;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = etx_open,
    .release        = etx_release,
    .unlocked_ioctl = etx_ioctl,
    .write          = etx_write,
};

static int __init etx_init(void)
{
    alloc_chrdev_region(&devnum, 0, 1, device_name);

    dev_class = class_create(THIS_MODULE, class_name);
    if (!dev_class)
        return -1;

    dev_device = device_create(dev_class, NULL, devnum, NULL, device_name);
    if (!dev_device)
        return -1;

    cdev_init(&etx_cdev, &fops);
    cdev_add(&etx_cdev, devnum, 1);

    printk(KERN_INFO "Loaded: %s (Class: %s) Major=%d Minor=%d\n",
           device_name, class_name, MAJOR(devnum), MINOR(devnum));

    return 0;
}

static void __exit etx_exit(void)
{
    device_destroy(dev_class, devnum);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(devnum, 1);

    printk(KERN_INFO "%s: Driver unloaded\n", device_name);
}

module_init(etx_init);
module_exit(etx_exit);

MODULE_LICENSE("GPL");
