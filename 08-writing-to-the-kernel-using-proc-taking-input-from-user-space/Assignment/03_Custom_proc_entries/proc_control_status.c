    #include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Module with /proc/control and /proc/status");

// Store current state
static char current_state[20] = "Idle";

#define MAX_LEN 100

/* ===========================
   /proc/status  (READ ONLY)
   =========================== */
ssize_t status_read(struct file *file, char __user *buf,
                    size_t count, loff_t *ppos)
{
    char message[64];
    sprintf(message, "Status: %s\n", current_state);

    return simple_read_from_buffer(buf, count, ppos,
                                   message, strlen(message));
}

/* ===========================
   /proc/control  (WRITE ONLY)
   =========================== */
ssize_t control_write(struct file *file, const char __user *buf,
                      size_t count, loff_t *ppos)
{
    char command[MAX_LEN];

    if (count > MAX_LEN - 1)
        count = MAX_LEN - 1;

    if (copy_from_user(command, buf, count))
        return -EFAULT;

    command[count] = '\0';

    // Process commands
    if (strncmp(command, "Start", 5) == 0) {
        strcpy(current_state, "Running");
        printk(KERN_INFO "Command: Start → Running\n");
    }
    else if (strncmp(command, "Stop", 4) == 0) {
        strcpy(current_state, "Stopped");
        printk(KERN_INFO "Command: Stop → Stopped\n");
    }
    else if (strncmp(command, "Pause", 5) == 0) {
        strcpy(current_state, "Paused");
        printk(KERN_INFO "Command: Pause → Paused\n");
    }
    else {
        printk(KERN_INFO "Unknown command: %s\n", command);
    }

    return count;
}

/* File ops */
static const struct proc_ops control_fops = {
    .proc_write = control_write,
};
static const struct proc_ops status_fops = {
    .proc_read = status_read,
};

/* Module Init */
static int __init proc_state_init(void)
{
    proc_create("control", 0666, NULL, &control_fops);
    proc_create("status", 0444, NULL, &status_fops);

    printk(KERN_INFO "/proc/control and /proc/status created\n");
    return 0;
}

/* Module Exit */
static void __exit proc_state_exit(void)
{
    remove_proc_entry("control", NULL);
    remove_proc_entry("status", NULL);

    printk(KERN_INFO "/proc/control and /proc/status removed\n");
}

module_init(proc_state_init);
module_exit(proc_state_exit);
