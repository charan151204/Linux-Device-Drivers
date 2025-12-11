#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Simulated platform device");

/* ---- Platform Data ---- */
struct my_pdata {
    int id;
    char label[20];
};

static struct my_pdata my_data = {
    .id = 5,
    .label = "SensorNode"
};

/* ---- Platform Device ---- */
static struct platform_device my_pdev = {
    .name = "myplatformdev",
    .id = -1,
};

static int __init my_platform_device_init(void)
{
    printk(KERN_INFO "[pdev] Registering platform device...\n");

    /* Attach platform data */
    my_pdev.dev.platform_data = &my_data;

    return platform_device_register(&my_pdev);
}

static void __exit my_platform_device_exit(void)
{
    printk(KERN_INFO "[pdev] Unregistering platform device...\n");
    platform_device_unregister(&my_pdev);
}

module_init(my_platform_device_init);
module_exit(my_platform_device_exit);
