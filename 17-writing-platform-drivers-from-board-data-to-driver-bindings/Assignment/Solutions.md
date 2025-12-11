2. Add Platform Data

In the device module:

static struct my_pdata {
    int id;
    char label[20];
} my_data = { .id = 5, .label = "SensorNode" };

my_pdev.dev.platform_data = &my_data;
In the driver’s probe():
struct my_pdata *pdata = dev_get_platdata(&pdev->dev);
printk(KERN_INFO "Data: id=%d, label=%s\n", pdata->id, pdata->label);

3. Simulate Two Devices
 
Register two platform devices with names "dev0" and "dev1"
Write a driver that handles both (via .of_match_table or .name)
Show both probe messages