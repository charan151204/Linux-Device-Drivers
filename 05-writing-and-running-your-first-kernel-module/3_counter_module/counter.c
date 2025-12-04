#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("A module that counts how many times it was loaded");

// Global variable to track module load count
static int counter;

// Module initialization function
static int __init counter_init(void)
{
    // Increment counter each time module is loaded
    counter++;
    // Log the current count to kernel log
    printk(KERN_INFO "counter module loaded. Current count = %d\n", counter);
    return 0;
}

// Module cleanup function
static void __exit counter_exit(void)
{
    // Log the final count when module is unloaded
    printk(KERN_INFO "counter module unloaded. Final count = %d\n", counter);
}

// Register module initialization function
module_init(counter_init);
// Register module cleanup function
module_exit(counter_exit);
