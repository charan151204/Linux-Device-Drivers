# Device Driver Assignments

## 1. Theory Review

### Why is `alloc_chrdev_region()` better than `register_chrdev()`?
`alloc_chrdev_region()` dynamically allocates major numbers automatically, avoiding conflicts and manual management. `register_chrdev()` requires manually specifying a major number, risking collisions with other drivers. Dynamic allocation is more scalable and safer for production kernels.

### What does `class_create()` actually do?
`class_create()` registers a new device class in sysfs under `/sys/class/`. It enables automatic device node creation through udev by establishing the device class hierarchy, allowing the kernel to communicate device metadata to userspace.

### What role does `udev` play in device node creation?
`udev` monitors kernel uevent messages and automatically creates device nodes in `/dev/` based on defined rules. It replaces static device files, enabling dynamic device discovery and management without manual intervention.

### What is cleaned by `device_destroy()`?
`device_destroy()` removes the device from sysfs and triggers a uevent that causes udev to delete the corresponding `/dev/` node. It cleans up the device's sysfs directory entry and associated kernel structures.

### What happens if `device_create()` fails?
If `device_create()` fails, it returns an error pointer (check with `IS_ERR()`). Recovery involves proper cleanup: unregister the character device, destroy the class, and deallocate the major number region using `unregister_chrdev_region()`.