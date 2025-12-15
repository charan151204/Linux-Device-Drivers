# Assignments

## 1. Quiz

### 1.1 What does GPIOD_OUT_LOW do?

GPIOD_OUT_LOW sets the GPIO pin to output low (0V).

### 1.2 Why must you call gpiod_put() in remove()?

gpiod_put() is necessary to release the GPIO pin, so it can be used by other drivers.

### 1.3 What happens if the GPIO pin is not defined in the DT?

The driver will not be able to find the GPIO pin, and the probe() function will return -ENODEV.

### 1.4 How can you toggle an LED from the driver at runtime?

You can toggle an LED from the driver at runtime by calling gpiod_set_value() with GPIOD_OUT_HIGH or GPIOD_OUT_LOW.

### 1.5 What does gpio-desc mean?

gpio-desc is a DT property that describes the GPIO pin used by the driver.

## 2. Add User Control via Sysfs (optional)

### 2.1 Add a sysfs entry under /sys/class/gpioled/control:

Write 1 → turn LED on

Write 0 → turn LED off

### 2.2 Use device_create_file() and store() callback to control the GPIO from user space.

## 3. Input GPIO (Bonus)

### 3.1 Modify the driver to:

Use an input GPIO (e.g., a button)
Read gpiod_get_value() inside a timer or workqueue
Print “Button Pressed” when value == 0