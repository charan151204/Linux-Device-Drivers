### 1. Theory Recap

#### 1.1 What does ioctl() allow that read() and write() do not?
##### ioctl() enables arbitrary device-specific control commands beyond simple data transfer

#### 1.2 Why do we need copy_to_user() or copy_from_user()?
##### To safely transfer data between user-space and kernel-space memory

#### 1.3 What does _IO vs _IOR mean?
##### `_IO`: Command with no data transfer
##### `_IOR`: Command that reads data from device to user-space

#### 1.4 What happens if the user passes an invalid cmd?
##### The driver should return `-EINVAL` or handle it gracefully

#### 1.5 Why is thread-safety still important in ioctl()?
##### Multiple processes may call ioctl() simultaneously, risking race conditions on shared driver state

## 2. Solution

### 2.1 New Command

#define IOCTL_SET_DEBUG _IOW(MAJOR_NUM, 2, int)

### 2.2 User Program Modification
