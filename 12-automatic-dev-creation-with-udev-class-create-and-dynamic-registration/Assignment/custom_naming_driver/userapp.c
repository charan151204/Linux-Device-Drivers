#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define MY_IOC_MAGIC 'k'
#define IOCTL_SET_DEBUG _IOW(MY_IOC_MAGIC, 3, int)

int main()
{
    int fd, debug;
    char msg[100];

    fd = open("/dev/autodev", O_WRONLY);
    if (fd < 0) {
        perror("Cannot open /dev/autodev");
        return -1;
    }

    printf("Enter debug mode (0 = OFF, 1 = ON): ");
    scanf("%d", &debug);

    if (ioctl(fd, IOCTL_SET_DEBUG, &debug) < 0)
        perror("ioctl");

    printf("Enter message to send: ");
    scanf("%s", msg);

    write(fd, msg, strlen(msg));

    close(fd);
    return 0;
}
