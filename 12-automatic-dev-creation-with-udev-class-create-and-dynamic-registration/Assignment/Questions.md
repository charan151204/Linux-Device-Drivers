# Device Driver Assignments

## 1. Theory Review

### Why is `alloc_chrdev_region()` better than `register_chrdev()`?
Explain the advantages of dynamic major number allocation over static registration.

### What does `class_create()` actually do?
Describe the role of `class_create()` in the device class hierarchy.

### What role does `udev` play in device node creation?
Discuss how udev automatically generates device nodes based on kernel events.

### What is cleaned by `device_destroy()`?
Identify the resources and structures removed by `device_destroy()`.

### What happens if `device_create()` fails?
Explain error handling and recovery strategies when device creation fails.

## 2. Custom Naming

### Modify the module so the device name (`autodev`) and class name (`myclass`) are accepted as module parameters.

Use the following module parameters:
```c
module_param(device_name, charp, 0660);
module_param(class_name, charp, 0660);
```

## 3. Dynamic Major Number Debugging

### After inserting your module:

1. Run `cat /proc/devices` to identify your device's major number
2. Use `udevadm info /dev/autodev` to trace device properties and verify configuration
