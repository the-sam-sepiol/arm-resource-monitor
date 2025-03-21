#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysinfo.h>
#include <linux/mm.h>  

#define PROC_NAME "ram_monitor"

/*Ram module for kernel*/

static int ram_monitor_show(struct seq_file *m, void *v)
{
    struct sysinfo info;
    si_meminfo(&info);

    seq_printf(m, "==========RAM STATS==========\n");
    seq_printf(m, "=============================\n");
    seq_printf(m, "Total RAM: %lu KB", info.totalram << (PAGE_SHIFT - 10));
    seq_printf(m, "\t%lu MB\n", info.totalram/1000 << (PAGE_SHIFT-10));
    seq_printf(m, "Free RAM:  %lu KB", info.freeram << (PAGE_SHIFT - 10));
    seq_printf(m, "\t%lu MB\n", info.freeram/1000 << (PAGE_SHIFT - 10));
    return 0;
}

static int ram_monitor_open(struct inode *inode, struct file *file)
{
    return single_open(file, ram_monitor_show, NULL);
}

static const struct proc_ops ram_monitor_ops = {
    .proc_open    = ram_monitor_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init ram_monitor_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &ram_monitor_ops);
    return 0;
}

static void __exit ram_monitor_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(ram_monitor_init);
module_exit(ram_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ethan Anderson");
MODULE_DESCRIPTION("RAM monitor for ARM systems (Kali Linux ARM 6.12).");
