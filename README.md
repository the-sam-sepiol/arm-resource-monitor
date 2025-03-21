## Resource Monitor for ARM Systems

This project provides a modular resource monitoring solution for ARM systems (tested on Kali Linux ARM 6.12). It consists of two main components:

- **Kernel Module (RAM Monitor):**  
  A kernel module (`ram_monitor.ko`) that exposes RAM usage statistics via `/proc/ram_monitor`.

- **User-Space CPU Monitor:**  
  A user-space program (`cpu_monitor`) that reads `/proc/stat` to compute per-core CPU usage and displays overall CPU metrics, including load averages.

A unified **Makefile** is provided to build both components using plain GCC. Additionally, several shell scripts automate running the monitors with periodic updates.

## Features

- **RAM Monitoring:**  
  The kernel module retrieves memory usage data.

- **CPU Monitoring:**  
  The user-space program parses `/proc/stat` to calculate individual core usage, average usage, maximum core usage, and displays the system load average from `/proc/loadavg`.

- **Build System:**  
  The shell scripts build both the kernel module and the CPU monitor using GCC.

- **Automation Scripts:**  
  - **`run_cpu.sh`:** Continuously displays only the CPU data (refreshes every 3 seconds).
  - **`run_continuous.sh`:** Continuously displays both CPU and RAM data (refreshes every 3–5 seconds with a random delay).
  - **`run_ram.sh`:** Continuously displays only the RAM data, refreshing instantly (i.e. without any delay).

## Requirements

- **GCC:**  
  Ensure GCC is installed for compiling the user-space program.

- **Kernel Headers:**  
  Ensure the kernel headers for your current kernel are installed (e.g., `/lib/modules/$(uname -r)/build` should exist).

- **Sudo Privileges:**  
  Required to insert and remove the kernel module.

## Setup

- **Clone the Repository:**

   ```bash
   git clone https://github.com/the-sam-sepiol/arm-resource-monitor.git
   cd resource-monitor-arm
   Run any automation script (.sh)
