#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <linux/timekeeping.h>

// static void *time_stamp_seq_start(struct seq_file *m, loff_t *pos)
// {
//     return NULL;
// }

// static void *time_stamp_seq_next(struct seq_file *m, void *v, loff_t *pos)
// {
//     return NULL;
// }

// static void time_stamp_seq_stop(struct seq_file *m, void *v) {}

// static int time_stamp_seq_show(struct seq_file *m, void *v)
// {
//     return 0;
// }

// static struct seq_operations time_stamp_seq_ops = {
//     .start  = time_stamp_seq_start,
//     .next   = time_stamp_seq_next,
//     .stop   = time_stamp_seq_stop,
//     .show   = time_stamp_seq_show,
// };

static int show_time_stamp(struct seq_file *m, void *v)
{
    struct timeval tv;
    do_gettimeofday(&tv);
    seq_printf(m, "Uptime (s): %lu\n", ktime_get_boottime_seconds());
    return 0;
}

static int open_time_stamp(struct inode *inode, struct file *filp)
{
    return single_open(inode, filp, show_time_stamp);
}

static const struct proc_ops time_stamp_fops = {
    .proc_open    = open_time_stamp,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init time_stamp_init(void)
{
    proc_create("time_stamp", 0, NULL, &time_stamp_fops);
    return 0;
}

static void __exit time_stamp_exit(void)
{
    remove_proc_entry("time_stamp");
}

module_init(time_stamp_init);
module_exit(time_stamp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charan");
MODULE_DESCRIPTION("Module displaying timestamp in /proc");
