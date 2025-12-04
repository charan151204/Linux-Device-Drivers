#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static int test_value = 10;
static char *author = "LinuxLearner";

module_param(test_value, int, 0660);     // int parameter
MODULE_PARM_DESC(test_value, "An integer value");

module_param(author, charp, 0660);       // char pointer (string) parameter
MODULE_PARM_DESC(author, "Author's name");

static int __init param_module_init(void)
{
    printk(KERN_INFO "param_module loaded.\n");
    printk(KERN_INFO "test_value = %d\n", test_value);
    printk(KERN_INFO "author = %s\n", author);
    return 0;
}

static void __exit param_module_exit(void)
{
    printk(KERN_INFO "param_module removed.\n");
}

module_init(param_module_init);
module_exit(param_module_exit);

 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Kernel Module with Parameters");