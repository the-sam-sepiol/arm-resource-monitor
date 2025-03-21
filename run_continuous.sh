#!/bin/bash
set -e

#clean previous builds if they exist
if [ -f "ram_monitor.ko" ] || [ -f "cpu_monitor" ]; then
        make clean
fi

make

#verify the kernel module exists
if [ ! -f "ram_monitor.ko" ]; then
    echo "Error: ram_monitor.ko not found. Check your build logs."
    exit 1
fi

if [ ! -f "cpu_monitor" ]; then
    echo "Error: cpu_monitor not found. Check your build logs."
    exit 1
fi

#if module is already loaded, remove it
if lsmod | grep -q "^ram_monitor"; then
       sudo rmmod ram_monitor
fi

#load module to kernel
sudo insmod ram_monitor.ko
while true; do
    clear
    echo "======RESOURCE  MONITOR======"
    echo "=======ENTER q TO QUIT======="
    echo ""
    cat /proc/ram_monitor
    echo ""
    ./cpu_monitor
    if read -t 3 -r input && [ "$input" = "q" ]; then
        break
    fi
done


echo "Cleaning up..."
make clean
