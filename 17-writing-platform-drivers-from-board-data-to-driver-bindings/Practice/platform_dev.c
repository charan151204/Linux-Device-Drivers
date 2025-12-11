#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Simulated platform device");

static struct platform_device my_pdev = {
    .name = "myplatformdev",
    .id = -1,
};

static int __init my_platform_driver_init(void) {
    printk(KERN_INFO "Registering platform device...\n");
    return platform_device_register(&my_pdev);
}

static void __exit my_platform_driver_exit(void) {
    printk(KERN_INFO "Unregistering platform device...\n");
    platform_device_unregister(&my_pdev);
}

module_init(my_platform_driver_init);
module_exit(my_platform_driver_exit);