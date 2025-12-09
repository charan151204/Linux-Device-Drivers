#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/**
 * Access a device and write a message to it, then read the response.
 * @param devname The name of the device to access.
 * @param msg The message to write to the device.
 */
void access_device(const char *devname, const char *msg)
{
    int fd;
    char buffer[200];

    // Print a message to indicate that we are accessing the device
    printf("\n--- Accessing %s ---\n", devname);

    // Open the device in read-write mode
    fd = open(devname, O_RDWR);
    if (fd < 0) {
        // If the open failed, print an error message
        perror(devname);
        return;
    }

    // Write the message to the device
    write(fd, msg, strlen(msg));

    // Move the file offset back to the start of the file
    lseek(fd, 0, SEEK_SET);

    // Read the response from the device
    int n = read(fd, buffer, sizeof(buffer)-1);
    buffer[n] = '\0';

    // Print the response
    printf("%s response:\n%s\n", devname, buffer);

    // Close the device
    close(fd);
}

/**
 * Main entry point for the program.
 * @return The exit status of the program.
 */
int main()
{
    // Access the first device
    access_device("/dev/mydev0", "Hello from dev0");

    // Access the second device
    access_device("/dev/mydev1", "Testing dev1");

    return 0;
}

