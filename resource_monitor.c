#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysinfo.h>
#include <linux/mm.h>         //for si_meminfo
#include <linux/smp.h>        //for for_each_online_cpu
#include <linux/sched/cputime.h>
#include <linux/kernel_stat.h>
#include <linux/percpu.h>

#define PROC_NAME "resource_monitor"

#define JIFFIES_TO_US(j) ((j) * (1000000ULL) / HZ)

static int resource_monitor_show(struct seq_file *m, void *v) {
    struct sysinfo info;
    int cpu;
    u64 idle_jiffies, idle_time_us;

    // Memory stats
    si_meminfo(&info);
    seq_printf(m, "=== Memory Stats ===\n");
    seq_printf(m, "====================\n");
    seq_printf(m, "Total RAM: %lu KB\n", info.totalram << (PAGE_SHIFT - 10));
    seq_printf(m, "Free RAM: %lu KB\n", info.freeram << (PAGE_SHIFT - 10));

    // CPU stats using cpustat on ARM:
    seq_printf(m, "\n=== CPU Stats ===\n");
    seq_printf(m, "=================\n");
    for_each_online_cpu(cpu) {
	idle_jiffies = per_cpu(kstat, cpu).cpustat[CPUTIME_IDLE];
	idle_time_us = JIFFIES_TO_US(idle_jiffies);
        //get the idle jiffies in the cpu
        seq_printf(m, "CPU %d Idle Time: %llu us\n", cpu, idle_time_us);
    }
    return 0;
}

static int resource_monitor_open(struct inode *inode, struct file *file) {
    return single_open(file, resource_monitor_show, NULL);
}

// Use the newer proc_ops structure for kernels 5.6 and later.
static const struct proc_ops resource_monitor_ops = {
    .proc_open    = resource_monitor_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init resource_monitor_init(void) {
    proc_create(PROC_NAME, 0, NULL, &resource_monitor_ops);
    printk(KERN_INFO "Module Online.\n");
    return 0;
}

static void __exit resource_monitor_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "Module Offline.\n");
}

module_init(resource_monitor_init);
module_exit(resource_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ethan Anderson");
MODULE_DESCRIPTION("Resource monitor for ARM systems (Specifically Kali Linux ARM 6.12)");
