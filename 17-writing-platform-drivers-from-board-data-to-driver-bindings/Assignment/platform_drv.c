#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Simple platform driver with platform_data");

struct my_pdata {
    int id;
    char label[20];
};

static int my_probe(struct platform_device *pdev)
{
    struct my_pdata *pdata;

    printk(KERN_INFO "[driver] Probe called for device: %s\n", pdev->name);

    /* Read platform data */
    pdata = dev_get_platdata(&pdev->dev);

    if (pdata)
        printk(KERN_INFO "[driver] Platform data: id=%d, label=%s\n",
               pdata->id, pdata->label);
    else
        printk(KERN_WARNING "[driver] No platform data!\n");

    return 0;
}

static int my_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "[driver] Remove called for device: %s\n", pdev->name);
    return 0;
}

static struct platform_driver my_platform_driver = {
    .driver = {
        .name = "myplatformdev",
        .owner = THIS_MODULE,
    },
    .probe = my_probe,
    .remove = my_remove,
};

static int __init pdrv_init(void)
{
    printk(KERN_INFO "[driver] Platform driver loaded\n");
    return platform_driver_register(&my_platform_driver);
}

static void __exit pdrv_exit(void)
{
    platform_driver_unregister(&my_platform_driver);
    printk(KERN_INFO "[driver] Platform driver unloaded\n");
}

module_init(pdrv_init);
module_exit(pdrv_exit);
