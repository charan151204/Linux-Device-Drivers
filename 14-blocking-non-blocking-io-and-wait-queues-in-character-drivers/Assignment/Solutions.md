# Assignments

## 1. Theory Recap

### Blocking vs Non-blocking I/O
**Blocking I/O**: The process sleeps until data is available. **Non-blocking I/O**: Returns immediately with -EAGAIN if data is unavailable.

### `wait_event_interruptible()` Without Condition
The process will sleep indefinitely or until a signal arrives, potentially causing a hang if the condition is never signaled.

### Mutex with Wait Queues
Mutex protects shared data from race conditions. The wait queue ensures only one process accesses the resource at a time, preventing data corruption.

### Multiple Readers Waiting
All waiting processes are woken when data arrives. They compete to acquire the mutex; only one proceeds while others re-enter the queue.

### Setting `O_NONBLOCK` in User Space
Use `open(path, O_RDONLY | O_NONBLOCK)` or `fcntl(fd, F_SETFL, O_NONBLOCK)` to set non-blocking mode.