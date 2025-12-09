## 1. Concept Review

**Major vs. Minor Numbers:**
- Major numbers identify the driver
- Minor numbers identify specific devices managed by that driver

**`container_of()` in `open()`:**
- Retrieves the device-specific structure from the inode's private data
- Allows access to device-specific buffers and state

**Shared `buffer[]` Issue:**
- Causes data corruption and race conditions
- Multiple devices would overwrite each other's data
- Solution: Each device needs its own buffer in a per-device structure

**`private_data` for Device Isolation:**
- Stores pointer to device-specific structure
- Kernel passes it to file operations automatically
- Ensures each open file instance accesses correct device data

**`MKDEV()` Macro:**
- Combines major and minor numbers into a `dev_t` value
- Required for `register_chrdev_region()` and device creation
- Syntax: `MKDEV(major, minor)`

