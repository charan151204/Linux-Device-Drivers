# Assignments

## 1. Concept Quiz

### Why is request_irq() preferred over polling?

request_irq() is preferred over polling because it allows the kernel to handle interrupts more efficiently and saves CPU cycles.

### What does IRQF_TRIGGER_FALLING do?

IRQF_TRIGGER_FALLING sets the interrupt to trigger on falling edge.

### What is the return type of an IRQ handler?

The return type of an IRQ handler is irqreturn_t.

### What happens if you don't call free_irq() in remove()?

If you don't call free_irq() in remove(), the interrupt resource will leak and the driver will not be able to release the interrupt.

### Can you sleep in an IRQ handler?

No, you cannot sleep in an IRQ handler.

## 2. Rising Edge Support

### Modify the driver to:

Accept a DT property "irq-trigger" (string: "rising", "falling", "both")

Parse it and choose IRQF_TRIGGER_RISING, etc., dynamically

## 3. Count Button Presses

### Add a press_count variable

### Increment it in the ISR

### Print total count on each press

Optional:

Expose it via sysfs or procfs so userspace can read the count
