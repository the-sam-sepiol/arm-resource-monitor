#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_CPUS 32

typedef struct {
    char label[16];
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
} cpu_stats;

/* Reads CPU statistics from /proc/stat.
 * This program reads from the /proc/stat in the kernel
 * It parses the data and then displays the calculations
 */
int read_cpu_stats(cpu_stats stats[], int max_entries) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    char line[MAX_LINE];
    int count = 0;
    while (fgets(line, sizeof(line), fp) != NULL && count < max_entries) {
        if (strncmp(line, "cpu", 3) == 0) {
            // Parse label and all numeric fields.
            sscanf(line, "%15s %llu %llu %llu %llu %llu %llu %llu %llu",
                   stats[count].label,
                   &stats[count].user,
                   &stats[count].nice,
                   &stats[count].system,
                   &stats[count].idle,
                   &stats[count].iowait,
                   &stats[count].irq,
                   &stats[count].softirq,
                   &stats[count].steal);
            count++;
        }
    }
    fclose(fp);
    return count;
}

/* returns the total time from all CPU fields for a given cpu_stats entry */
unsigned long long total_time(const cpu_stats *s) {
    return s->user + s->nice + s->system + s->idle +
           s->iowait + s->irq + s->softirq + s->steal;
}

/* computes CPU usage percentage between two snapshots for one cpu_stats entry */
double compute_usage(const cpu_stats *prev, const cpu_stats *curr){
    unsigned long long prev_total = total_time(prev);
    unsigned long long curr_total = total_time(curr);
    unsigned long long total_diff = curr_total - prev_total;
    unsigned long long idle_prev = prev->idle + prev->iowait;
    unsigned long long idle_curr = curr->idle + curr->iowait;
    unsigned long long idle_diff = idle_curr - idle_prev;
    double usage = 0.0;
    if (total_diff > 0)
        usage = 100.0 * (total_diff - idle_diff) / total_diff;
    return usage;
}

int main(){
    cpu_stats prev_stats[MAX_CPUS], curr_stats[MAX_CPUS];
    int count = read_cpu_stats(prev_stats, MAX_CPUS);
    if (count < 1) {
        fprintf(stderr, "Error reading /proc/stat\n");
        return EXIT_FAILURE;
    }
    sleep(1); 
    int count2 = read_cpu_stats(curr_stats, MAX_CPUS);
    if (count2 < count)
        count = count2;
    
    double sum_usage = 0.0;
    double max_usage = 0.0;
    int core_count = 0;
    printf("=====CPU Usage Per Core =====\n");
    //process per-core entries    
    for (int i = 0; i < count; i++) {
        if (strcmp(prev_stats[i].label, "cpu") == 0 || strcmp(prev_stats[i].label, "all") == 0) {
            continue;
        }
        double usage = compute_usage(&prev_stats[i], &curr_stats[i]);
        printf("%s Usage: %.2f%%\n", prev_stats[i].label, usage);
        sum_usage += usage;
        if (usage > max_usage)
            max_usage = usage;
        core_count++;
    }
    
    double average_usage = (core_count > 0) ? (sum_usage / core_count) : 0.0;
    
    // compute the overall usage using the aggregated line (if available)
    double overall_usage = 0.0;
    int found_agg = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(prev_stats[i].label, "cpu") == 0 || strcmp(prev_stats[i].label, "all") == 0) {
            overall_usage = compute_usage(&prev_stats[i], &curr_stats[i]);
            found_agg = 1;
            break;
        }
    }
    
    printf("\nAvg CPU Usage (individual cores): %.2f%%\n", average_usage);
    printf("Maximum CPU Usage (all cores): %.2f%%\n", max_usage);
    if (found_agg)
        printf("Overall CPU Usage (aggregate): %.2f%%\n", overall_usage);
    
    // print load average
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp) {
        char loadavg[128];
        if (fgets(loadavg, sizeof(loadavg), fp))
            printf("\nLoad Average: %s", loadavg);
        fclose(fp);
    }
    
    return EXIT_SUCCESS;
}
