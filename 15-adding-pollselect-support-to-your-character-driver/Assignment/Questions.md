# Assignments

## 1. Concept Quiz

- What is the purpose of `poll_wait()` in the driver?
- What does the `poll()` system call do in user space?
- What happens if your driver's `.poll` returns 0?
- How does `poll()` differ from blocking `read()`?
- Why is `POLLIN` used in the return mask?

## 2. Support POLLOUT

Add logic to support `POLLOUT` in `.poll` if your driver is ready to accept a write.
Define a `can_write` flag and return `POLLOUT` if it's set.

## 3. Combine with ioctl

Let user space use `ioctl()` to set a flag like `READ_READY`.
Then your `.poll` will return `POLLIN` only if that flag is set.