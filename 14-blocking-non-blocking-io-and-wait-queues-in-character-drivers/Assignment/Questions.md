# Assignments

## 1. Theory Recap

- What's the difference between blocking and non-blocking I/O?
- What happens if `wait_event_interruptible()` is used without a condition?
- Why do we need mutex along with wait queues?
- What happens if multiple readers are waiting?
- How does user space set `O_NONBLOCK`?

## 2. Add Multi-Reader Support

Modify your driver to:

- Add a `reader_count`
- Track how many times readers were blocked and resumed
- Show this count in `dmesg`

## 3. Simple C App

Write a C user program that:

- Opens the device
- Reads (blocking) in one thread
- Writes after 3 seconds in another thread
- Demonstrates synchronization
