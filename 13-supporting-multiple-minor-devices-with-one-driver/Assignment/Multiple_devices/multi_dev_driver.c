#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define DEVICE_COUNT 2              /* Number of devices to create */
#define MAX_MSG 100                 /* Maximum message buffer size */

/**
 * Device structure to hold per-device data
 */
typedef struct {
    int write_count;                /* Count of write operations */
    char last_msg[MAX_MSG];         /* Buffer for last written message */
    struct cdev cdev;               /* Character device structure */
} mydev_t;

/* Global variables */
static dev_t devnum;                /* Device number (major, minor) */
static struct class *dev_class;     /* Device class for sysfs */
static mydev_t devices[DEVICE_COUNT]; /* Array of device instances */

/* ============ FILE OPERATIONS ============ */

/**
 * my_open - Called when device file is opened
 */
static int my_open(struct inode *inode, struct file *file)
{
    mydev_t *dev;

    /* Extract device structure from inode */
    dev = container_of(inode->i_cdev, mydev_t, cdev);
    file->private_data = dev;

    return 0;
}

/**
 * my_write - Write data to device
 */
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    mydev_t *dev = file->private_data;

    /* Limit message length to buffer size */
    if (len >= MAX_MSG)
        len = MAX_MSG - 1;

    /* Copy data from user space */
    if (copy_from_user(dev->last_msg, buf, len))
        return -EFAULT;

    dev->last_msg[len] = '\0';
    dev->write_count++;

    return len;
}

/**
 * my_read - Read device data and statistics
 */
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    char temp[200];
    int n;
    mydev_t *dev = file->private_data;

    /* Format device information */
    n = snprintf(temp, sizeof(temp),
                 "Write count = %d\nLast message = %s\n",
                 dev->write_count, dev->last_msg);

    /* Handle end of file */
    if (*off >= n)
        return 0;

    /* Adjust read length */
    if (len > n - *off)
        len = n - *off;

    /* Copy data to user space */
    if (copy_to_user(buf, temp + *off, len))
        return -EFAULT;

    *off += len;
    return len;
}

/**
 * File operations structure
 */
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .read    = my_read,
    .write   = my_write,
};

/* ============ MODULE INITIALIZATION ============ */

/**
 * my_init - Module initialization
 */
static int __init my_init(void)
{
    int i;

    /* Register character device region */
    alloc_chrdev_region(&devnum, 0, DEVICE_COUNT, "mydev");

    /* Create device class */
    dev_class = class_create(THIS_MODULE, "myclass");

    /* Initialize each device */
    for (i = 0; i < DEVICE_COUNT; i++) {
        cdev_init(&devices[i].cdev, &fops);
        cdev_add(&devices[i].cdev, MKDEV(MAJOR(devnum), i), 1);

        /* Initialize device state */
        devices[i].write_count = 0;
        strcpy(devices[i].last_msg, "<none>");

        /* Create device file in sysfs */
        device_create(dev_class, NULL,
                      MKDEV(MAJOR(devnum), i),
                      NULL,
                      "mydev%d", i);
    }

    printk(KERN_INFO "Loaded %d devices: /dev/mydev0 ...\n", DEVICE_COUNT);
    return 0;
}

/**
 * my_exit - Module cleanup
 */
static void __exit my_exit(void)
{
    int i;

    /* Destroy all devices */
    for (i = 0; i < DEVICE_COUNT; i++) {
        device_destroy(dev_class, MKDEV(MAJOR(devnum), i));
        cdev_del(&devices[i].cdev);
    }

    class_destroy(dev_class);
    unregister_chrdev_region(devnum, DEVICE_COUNT);

    printk(KERN_INFO "Driver unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
