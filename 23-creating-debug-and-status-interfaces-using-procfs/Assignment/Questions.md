# Assignments

## 1. Quiz

### Where does /proc exist on a Linux system?

/proc is a virtual filesystem that provides information about the system, processes, and kernel modules.

### What's the role of seq_file in procfs?

seq_file is a helper in kernel to write output line-by-line in a memory-safe way.

### Can /proc/gpio/button_info be used to write data?

No, /proc/gpio/button_info is read-only.

### Why might you prefer procfs over printk logs?

procfs is preferred over printk logs because it doesn't pollute the kernel logs and provides a more structured way to communicate with user space.

### What happens if proc_remove() is skipped?

If proc_remove() is skipped, the proc entry will remain after the module is removed, which can lead to dangling pointers, potential kernel crashes.

## 2. Add Timestamp

Add the following line to the proc file output:

Uptime (s): `ktime_get_boottime_seconds()`

## 3. Add Multi-Device Support

If you had multiple buttons, create the following files:

/proc/gpio/btn0_info

/proc/gpio/btn1_info

Each with its own IRQ, count, LED state.
