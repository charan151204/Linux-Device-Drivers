# Kernel IOCTL Custom Commands - Character Drivers Assignment

## 1. Theory Recap

- **What does ioctl() allow that read() and write() do not?**
    - ioctl() enables arbitrary device-specific control commands beyond simple data transfer

- **Why do we need copy_to_user() or copy_from_user()?**
    - To safely transfer data between user-space and kernel-space memory

- **What does _IO vs _IOR mean?**
    - `_IO`: Command with no data transfer
    - `_IOR`: Command that reads data from device to user-space

- **What happens if the user passes an invalid cmd?**
    - The driver should return `-EINVAL` or handle it gracefully

- **Why is thread-safety still important in ioctl()?**
    - Multiple processes may call ioctl() simultaneously, risking race conditions on shared driver state

## 2. Add Your Own Command

Create a new `#define IOCTL_SET_DEBUG _IOW(...)` and:
- Pass a 0 or 1 to enable/disable a `debug_mode` flag in the driver
- In `write()`, if `debug_mode == 1`, use `printk()` to log extra info

## 3. Extend User Program

Modify the user program to:
- Toggle debug mode using the new command
- Write new data
- Confirm behavior by checking `dmesg`