/*
 * waitqueue_driver.c
 *
 * This program demonstrates the use of a wait queue in a character
 * driver. It creates a character device which can be used to read
 * from and write to. The read function blocks until data is available,
 * and the write function wakes up any blocked readers.
 *
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/mutex.h>

static dev_t devnum; /* Device number */
static struct cdev my_cdev; /* Character device structure */

/* Wait queue for blocked readers */
static DECLARE_WAIT_QUEUE_HEAD(read_queue);

/* Mutex to protect shared data */
static DEFINE_MUTEX(lock);

/* Flag to indicate data availability */
static int data_ready = 0;

/* Count of readers currently blocked */
static int reader_count = 0;

/* Count of readers resumed */
static int resume_count = 0;

/*
 * my_read - Read data from the device
 *
 * This function blocks until data is available, then returns the data to
 * the caller.
 */
static ssize_t my_read(struct file *file, char __user *buf,size_t len, loff_t *off)
{
    char msg[] = "Data ready!\n";

    mutex_lock(&lock);

    /* If data is not available, block and wait */
    if (!data_ready) {
        reader_count++;
        mutex_unlock(&lock);

        if (wait_event_interruptible(read_queue, data_ready))
            return -ERESTARTSYS;

        resume_count++;
        printk(KERN_INFO
               "Readers blocked=%d, resumed=%d\n",
               reader_count, resume_count);

        mutex_lock(&lock);
    }

    /* Reset data available flag */
    data_ready = 0;

    mutex_unlock(&lock);

    /* Copy data to user space */
    if (copy_to_user(buf, msg, sizeof(msg)))
        return -EFAULT;

    return sizeof(msg);
}

/*
 * my_write - Write data to the device
 *
 * This function wakes up any blocked readers and makes data available.
 */
static ssize_t my_write(struct file *file,const char __user *buf,size_t len, loff_t *off)
{
    mutex_lock(&lock);
    data_ready = 1;
    mutex_unlock(&lock);

    wake_up_interruptible(&read_queue);

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = my_read,
    .write = my_write,
};

/*
 * my_init - Module initialization function
 */
static int __init my_init(void)
{
    /* Allocate device number */
    alloc_chrdev_region(&devnum, 0, 1, "waitqdev");

    /* Initialize character device structure */
    cdev_init(&my_cdev, &fops);

    /* Add character device to the system */
    cdev_add(&my_cdev, devnum, 1);

    printk(KERN_INFO "waitqueue driver loaded: major=%d\n",
           MAJOR(devnum));

    return 0;
}

/*
 * my_exit - Module cleanup function
 */
static void __exit my_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(devnum, 1);
    printk(KERN_INFO "Driver unloaded\n");
}

/* Module initialization and cleanup */
module_init(my_init);
module_exit(my_exit);

/* Module license */
MODULE_LICENSE("GPL");

