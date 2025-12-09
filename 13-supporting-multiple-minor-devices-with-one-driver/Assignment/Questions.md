# Assignments

## 1. Concept Review

- What's the difference between major and minor numbers?
- Why do we use `container_of()` in `open()`?
- What happens if two devices share the same `buffer[]`?
- How does `private_data` help isolate devices?
- What is `MKDEV()` and why is it needed?

## 2. Add Per-Device Counters

- Add a `write_count` variable to each `mydev_t` struct
- Increment it on each `write()`
- Return it in `read()` as: `Write count = X`
- Last message: `<message>`

## 3. Write a C User App

Write a user-space C program that:

- Opens `/dev/mydev0`, `/dev/mydev1`, etc.
- Writes to each device
- Reads and prints the results
