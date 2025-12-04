#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Module that takes an array of 3 integers and prints the average");

// Array of 3 integers
static int arr[3] = {0, 0, 0};
static int count = 0;   // Will store how many values user actually passed

// Register array parameter
module_param_array(arr, int, &count, 0660);
MODULE_PARM_DESC(arr, "Array of 3 integers");

static int __init array_module_init(void)
{
    int sum = 0, i;

    printk(KERN_INFO "array_module loaded.\n");
    printk(KERN_INFO "Values received = %d\n", count);

    // Sum the elements
    for (i = 0; i < count; i++)
        sum += arr[i];

    if (count > 0)
        printk(KERN_INFO "Average = %d\n", sum / count);
    else
        printk(KERN_INFO "No values provided.\n");

    return 0;
}

static void __exit array_module_exit(void)
{
    printk(KERN_INFO "array_module unloaded.\n");
}

module_init(array_module_init);
module_exit(array_module_exit);
