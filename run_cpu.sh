#!/bin/bash
set -e

#clean previous builds if they exist
if [ -f "cpu_monitor" ]; then
        make clean
fi

make

if [ ! -f "cpu_monitor" ]; then
    echo "Error: cpu_monitor not found. Check your build logs."
    exit 1
fi
while true; do
    clear
    echo "=========CPU MONITOR========="
    echo "=======ENTER q TO QUIT======="
    echo ""
    ./cpu_monitor
    if read -t 5 -r input && [ "$input" = "q" ]; then
        break
    fi

done




echo "Cleaning up..."
make clean
