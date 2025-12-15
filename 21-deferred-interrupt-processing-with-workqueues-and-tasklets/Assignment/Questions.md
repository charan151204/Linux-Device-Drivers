# Assignments

## 1. Quiz

### 1.1 Why must you not sleep in an ISR?

Because ISRs are not allowed to sleep or block, and sleeping in an ISR can cause a system crash.

### 1.2 What happens if you call msleep() in an IRQ handler?

The system will crash or become unstable, because ISRs are not allowed to block or sleep.

### 1.3 What is the benefit of a workqueue over tasklets?

Workqueues are more efficient than tasklets because they can handle more complex tasks and can be used to queue work items from interrupt handlers.

### 1.4 What does cancel_work_sync() ensure?

cancel_work_sync() ensures that all work items queued are completed before returning.

### 1.5 Can you call gpiod_get_value() inside a workqueue?

Yes, you can call gpiod_get_value() inside a workqueue. Workqueues can call blocking functions like gpiod_get_value().

## 2. Toggle LED on Button Press (in Workqueue)

### 2.1 Add an output GPIO (led_gpiod)

Add an output GPIO with the name "led_gpiod" to toggle an LED on button press.

### 2.2 In btn_work_handler(), toggle LED state

In btn_work_handler(), toggle the LED state using gpiod_set_value().

### 2.3 Use a static flag to keep track of ON/OFF

Use a static flag to keep track of the LED state (ON/OFF).

## 3. Bounce Filter

### 3.1 In btn_work_handler(), ignore presses if last press was within 100ms

In btn_work_handler(), ignore presses if the last press was within 100ms by using ktime_get() or jiffies to track time between presses.

### 3.2 Use ktime_get() or jiffies to track time between presses

Use ktime_get() or jiffies to track time between presses and implement a bounce filter.

