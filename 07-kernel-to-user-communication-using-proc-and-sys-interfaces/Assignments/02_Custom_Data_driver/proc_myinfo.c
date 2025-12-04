#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>

#define PROC_NAME "myinfo"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Simple /proc module showing uptime and counter");

// Module parameter (string)
static char *author = "default";
module_param(author, charp, 0660);

// Counter that increases every time file is read
static int read_counter = 0;

static int show_myinfo(struct seq_file *m, void *v)
{
    unsigned long uptime_sec = jiffies / HZ;

    read_counter++;  // Increase every time user reads /proc/myinfo

    seq_printf(m, "Author       : %s\n", author);
    seq_printf(m, "Uptime (sec) : %lu\n", uptime_sec);
    seq_printf(m, "Read Count   : %d\n", read_counter);

    return 0;
}

static int open_myinfo(struct inode *inode, struct file *file)
{
    return single_open(file, show_myinfo, NULL);
}

static const struct proc_ops myinfo_fops = {
    .proc_open    = open_myinfo,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init proc_myinfo_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &myinfo_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit proc_myinfo_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_myinfo_init);
module_exit(proc_myinfo_exit);
