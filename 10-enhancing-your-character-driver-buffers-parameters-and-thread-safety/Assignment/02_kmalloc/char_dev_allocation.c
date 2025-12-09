#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Dynamic ring buffer using kmalloc");

#define DEVICE_NAME "enhchardev"
#define MSG_LEN 128 /* Size of each message buffer */

static int major;
static int max_messages = 5; /* Default number of messages in ring buffer */
module_param(max_messages, int, 0660);

static char **ring_buffer; /* Dynamic array of pointers to message buffers */
static int write_index = 0; /* Current write position in ring buffer */

/* ------------ WRITE OPERATION ------------ */
/**
 * dev_write - Write data to the ring buffer
 * Copies user data to the current buffer slot and advances the write index
 */
static ssize_t dev_write(struct file *file, const char __user *buf,
                         size_t len, loff_t *offset)
{
    /* Limit write length to buffer size minus null terminator */
    if (len > MSG_LEN - 1)
        len = MSG_LEN - 1;

    /* Copy data from user space to kernel space */
    if (copy_from_user(ring_buffer[write_index], buf, len))
        return -EFAULT;

    /* Null-terminate the message */
    ring_buffer[write_index][len] = '\0';

    printk(KERN_INFO "Saved message[%d]: %s\n", write_index, ring_buffer[write_index]);

    /* Move to next slot (wrap around using modulo) */
    write_index = (write_index + 1) % max_messages;
    return len;
}

/* ------------ READ OPERATION ------------ */
/**
 * dev_read - Read status message from device
 */
static ssize_t dev_read(struct file *file, char __user *buf,
                        size_t len, loff_t *offset)
{
    const char *msg = "Dynamic ring buffer active\n";
    return simple_read_from_buffer(buf, len, offset, msg, strlen(msg));
}

/* ------------ OPEN / RELEASE OPERATIONS ------------ */
/**
 * dev_open - Called when device is opened
 */
static int dev_open(struct inode *inode, struct file *file)
{
    return 0;
}

/**
 * dev_release - Called when device is closed
 */
static int dev_release(struct inode *inode, struct file *file)
{
    return 0;
}

/* File operations structure */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

/* ------------ MODULE INITIALIZATION ------------ */
/**
 * ringbuf_init - Initialize the character device and allocate ring buffer
 * Returns 0 on success, negative error code on failure
 */
static int __init ringbuf_init(void)
{
    int i;

    printk(KERN_INFO "Allocating ring buffer of %d messages\n", max_messages);

    /* Allocate array of char pointers */
    ring_buffer = kmalloc(max_messages * sizeof(char *), GFP_KERNEL);

    if (!ring_buffer)
        return -ENOMEM;

    /* Allocate memory for each message slot */
    for (i = 0; i < max_messages; i++) {
        ring_buffer[i] = kmalloc(MSG_LEN, GFP_KERNEL);

        if (!ring_buffer[i]) {
            printk(KERN_ERR "kmalloc failed at index %d\n", i);

            /* Free previously allocated slots on failure */
            while (--i >= 0)
                kfree(ring_buffer[i]);

            kfree(ring_buffer);
            return -ENOMEM;
        }
    }

    /* Register character device */
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "Failed to register device\n");
        return major;
    }

    printk(KERN_INFO "Driver registered with major number %d\n", major);

    return 0;
}

/* ------------ MODULE EXIT ------------ */
/**
 * ringbuf_exit - Cleanup and unregister character device
 */
static void __exit ringbuf_exit(void)
{
    int i;

    printk(KERN_INFO "Freeing ring buffer\n");

    /* Free each message buffer */
    for (i = 0; i < max_messages; i++)
        kfree(ring_buffer[i]);

    /* Free the pointer array */
    kfree(ring_buffer);

    /* Unregister the character device */
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Ring buffer driver unloaded\n");
}

module_init(ringbuf_init);
module_exit(ringbuf_exit);
