#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Module with parameters: author and repeat_count");

// String parameter
static char *author = "default";
module_param(author, charp, 0660);
MODULE_PARM_DESC(author, "Author name");

// Integer parameter
static int repeat_count = 1;
module_param(repeat_count, int, 0660);
MODULE_PARM_DESC(repeat_count, "Number of times to print author name");

static int __init param_module_init(void)
{
    int i;

    for (i = 0; i < repeat_count; i++)
        printk(KERN_INFO "Author = %s\n", author);

    return 0;
}

static void __exit param_module_exit(void)
{
    printk(KERN_INFO "param_module removed.\n");
}

module_init(param_module_init);
module_exit(param_module_exit);
