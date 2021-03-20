#!/bin/bash
make
echo ">>>>> Loading md1.ko..."
echo
sleep 0.8
insmod md1.ko
lsmod | grep md
echo
sleep 1
echo ">>>>> This is the last 15 lines in the system log: "
dmesg | tail -5
echo ">>>>> Remove md1..."
rmmod md1
echo
sleep 0.8
dmesg | tail -5
