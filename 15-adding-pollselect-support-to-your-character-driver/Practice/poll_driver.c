#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/mutex.h>

#define DEVICE_NAME "polldev"
#define CLASS_NAME  "pollclass"
#define BUF_LEN 256

// Static global variables
static dev_t dev_num;
static struct cdev poll_cdev;
static struct class *poll_class;
static struct device *poll_device;

// Buffer for storing data written to the device
static char buffer[BUF_LEN];
// Flag to indicate if data is available for reading
static int data_available = 0;

// Wait queue for blocking readers
static DECLARE_WAIT_QUEUE_HEAD(wq);
// Mutex to protect shared data
static DEFINE_MUTEX(lock);

// File operations

/**
 * @brief Open the device
 * @param inode Pointer to the inode of the device
 * @param file Pointer to the file structure
 * @return 0 on success, negative error code on failure
 */
static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] opened\n", DEVICE_NAME);
    return 0;
}

/**
 * @brief Release the device
 * @param inode Pointer to the inode of the device
 * @param file Pointer to the file structure
 * @return 0 on success, negative error code on failure
 */
static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] closed\n", DEVICE_NAME);
    return 0;
}

/**
 * @brief Read from the device
 * @param file Pointer to the file structure
 * @param buf Pointer to the user-space buffer to store the read data
 * @param len Size of the user-space buffer
 * @param off Pointer to the offset in the file
 * @return Number of bytes read on success, negative error code on failure
 */
static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    int ret;

    // Block until data is available
    wait_event_interruptible(wq, data_available);

    // Lock the mutex to protect shared data
    mutex_lock(&lock);
    // Read data from the buffer
    ret = simple_read_from_buffer(buf, len, off, buffer, strlen(buffer));
    // Reset the data available flag
    data_available = 0;
    // Unlock the mutex
    mutex_unlock(&lock);

    return ret;
}

/**
 * @brief Write to the device
 * @param file Pointer to the file structure
 * @param buf Pointer to the user-space buffer containing the write data
 * @param len Size of the user-space buffer
 * @param off Pointer to the offset in the file
 * @return Number of bytes written on success, negative error code on failure
 */
static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    // Check if the length of the write data is within the buffer size
    if (len > BUF_LEN - 1)
        len = BUF_LEN - 1;

    // Lock the mutex to protect shared data
    mutex_lock(&lock);
    // Copy the write data to the buffer
    if (copy_from_user(buffer, buf, len)) {
        mutex_unlock(&lock);
        return -EFAULT;
    }

    // Null-terminate the buffer
    buffer[len] = '\0';
    // Set the data available flag
    data_available = 1;
    // Unlock the mutex
    mutex_unlock(&lock);

    // Wake up any blocked readers
    wake_up_interruptible(&wq);
    printk(KERN_INFO "[%s] wrote: %s\n", DEVICE_NAME, buffer);
    return len;
}

/**
 * @brief Poll the device
 * @param file Pointer to the file structure
 * @param wait Pointer to the poll table
 * @return Bitmask of events that are ready
 */
static __poll_t dev_poll(struct file *file, poll_table *wait)
{
    __poll_t mask = 0;

    // Register the file to the wait queue
    poll_wait(file, &wq, wait);

    // Check if data is available for reading
    if (data_available)
        mask |= POLLIN | POLLRDNORM;  // Data available for reading

    return mask;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .release = dev_release,
    .read    = dev_read,
    .write   = dev_write,
    .poll    = dev_poll,
};

static int __init poll_driver_init(void)
{
    int ret;

    // Allocate the device number
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    // Initialize the character device
    cdev_init(&poll_cdev, &fops);
    cdev_add(&poll_cdev, dev_num, 1);

    // Create the device class and device
    poll_class = class_create(CLASS_NAME);
    poll_device = device_create(poll_class, NULL, dev_num, NULL, DEVICE_NAME);

    printk(KERN_INFO "[%s] loaded\n", DEVICE_NAME);
    return 0;
}

static void __exit poll_driver_exit(void)
{
    // Destroy the device class and device
    device_destroy(poll_class, dev_num);
    class_destroy(poll_class);
    // Delete the character device
    cdev_del(&poll_cdev);
    // Unregister the device number
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "[%s] unloaded\n", DEVICE_NAME);
}

module_init(poll_driver_init);
module_exit(poll_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Character driver with poll() support");
