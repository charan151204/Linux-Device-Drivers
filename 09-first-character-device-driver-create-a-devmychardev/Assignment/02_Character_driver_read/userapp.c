#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char buffer[100];

    fd = open("/dev/mychardev", O_RDWR);
    if (fd < 0) 
    {
        perror("open");
        return 1;
    }

    write(fd, "Testing from app", 16);
    read(fd, buffer, sizeof(buffer));

    printf("Kernel says: %s\n", buffer);

    close(fd);
    return 0;
}
