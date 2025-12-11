## 1. Concept Quiz

### Answers

- **What is the purpose of `poll_wait()` in the driver?**
    - `poll_wait()` registers the caller with the wait queue, allowing the driver to wake them up when data becomes available without blocking.

- **What does the `poll()` system call do in user space?**
    - `poll()` monitors multiple file descriptors to see if they're ready for I/O operations, returning immediately with status information.

- **What happens if your driver's `.poll` returns 0?**
    - Returning 0 indicates no events are currently available, so `poll()` will block or timeout waiting for the driver to signal readiness.

- **How does `poll()` differ from blocking `read()`?**
    - `poll()` checks readiness without consuming data and can monitor multiple descriptors simultaneously, while `read()` blocks on a single descriptor until data is available.

- **Why is `POLLIN` used in the return mask?**
    - `POLLIN` signals that the file descriptor is readable and data is available, allowing `poll()` to return and notify the user space application.

