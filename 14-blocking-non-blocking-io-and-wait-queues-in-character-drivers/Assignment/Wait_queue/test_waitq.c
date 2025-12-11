#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/**
 * Test program for wait queue device driver
 */
int main()
{
    int fd = open("/dev/waitqdev", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int pid = fork();

    if (pid == 0) {
        // Child: Reader
        char buf[50];
        printf("Reader: calling read()...\n");
        // Read() will block until data is written to the device
        int n = read(fd, buf, sizeof(buf));
        buf[n] = '\0';

        printf("Reader: woke up! Message: %s\n", buf);
        return 0;
    }
    else {
        // Parent: Writer
        // Simulate some delay before writing to the device
        sleep(3);
        printf("Writer: writing to device...\n");
        // Write a single character to the device
        write(fd, "X", 1);
    }

    return 0;
}

