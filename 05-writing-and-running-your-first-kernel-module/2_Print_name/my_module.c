// 2. Change your module to:
// Print Charan and current time when loading
// Print a goodbye message with module name when unloading
// Hint:
// Use printk(KERN_INFO "Author:  Charan>\n");
// Use #include <linux/timekeeping.h> and ktime_get_real_seconds() for time (optional)

// Include the module header file for kernel module functionality
#include <linux/module.h>
// Include initialization macros for __init and __exit
#include <linux/init.h>
// Include kernel logging functions like printk
#include <linux/kernel.h>
// Include time-related structures and functions
#include <linux/time.h>

// Declare the license type for the module
MODULE_LICENSE("GPL");
// Declare the author of the module
MODULE_AUTHOR("Charan");
// Declare a description of what the module does
MODULE_DESCRIPTION("Module printing name and time on load, goodbye on unload");

// Module initialization function
static int __init my_module_init(void)
{
    // Declare timespec64 structure for current time
    struct timespec64 ts;
    // Get current real time
    ktime_get_real_ts64(&ts);

    // Print author name on module load
    printk(KERN_INFO "Author: Charan\n");
    // Print current time in seconds
    printk(KERN_INFO "Module loaded at: %lld seconds\n", ts.tv_sec);

    // Return success
    return 0;
}

// Module exit function
static void __exit my_module_exit(void)
{
    // Print goodbye message with module name
    printk(KERN_INFO "Goodbye from module: %s\n", THIS_MODULE->name);
}

// Register init function
module_init(my_module_init);
// Register exit function
module_exit(my_module_exit);
