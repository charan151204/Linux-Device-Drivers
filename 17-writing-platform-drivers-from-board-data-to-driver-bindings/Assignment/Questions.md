# Assignments

## 1. Quiz

### 1. What triggers the call to `probe()` in a platform driver?
The call to `probe()` is triggered by the kernel when it finds a device that matches the driver's `of_match_table`.

### 2. Can you have multiple platform devices with the same name?
No, multiple platform devices with the same name cannot exist.

### 3. What does `platform_driver_unregister()` do?
`platform_driver_unregister()` registers a platform driver with the kernel.

### 4. What is the significance of `.id = -1` in a platform device?
`.id = -1` is used to indicate that the platform device does not have a device tree node.

### 5. Where is `pdev->name` compared during matching?
`pdev->name` is compared during matching in the `probe()` function of the platform driver.


