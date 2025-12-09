#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd = open("/dev/enhchardev", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char *lines[] = {"Line 1\n", "Line 2\n", "Line 3\n"};
    int num_lines = sizeof(lines) / sizeof(lines[0]);

    for (int i = 0; i < num_lines; i++)
    {
        write(fd, lines[i], strlen(lines[i]));
    }

    char buffer[MSG_LEN * MAX_MSGS];
    int read_bytes = read(fd, buffer, MSG_LEN * MAX_MSGS);

    if (read_bytes == -1)
    {
        perror("read");
        return 1;
    }

    buffer[read_bytes] = '\0'; // null terminate

    printf("%s", buffer);

    close(fd);
    return 0;
}
