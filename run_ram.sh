#!/bin/bash
set -e

#clean previous builds if they exist
if [ -f "ram_monitor.ko" ]; then
        make clean
fi

make

#verify the kernel module exists
if [ ! -f "ram_monitor.ko" ]; then
    echo "ram_monitor.ko was not found."
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
    echo "=========RAM MONITOR========="
    echo "=======ENTER q TO QUIT======="
    echo ""
    cat /proc/ram_monitor
    echo ""    
    if read -t 3 -r input && [ "$input" = "q" ]; then
        break
    fi
done




echo "Cleaning up..."
make clean
