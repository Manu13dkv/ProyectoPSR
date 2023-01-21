#!/bin/bash

n_nodes=$1
n_taps=$(( n_nodes * 2 ))

for i in $(seq 1 $n_taps)
do
    sudo ip link delete tap${i}
done


for i in $(seq 1 $n_nodes)
do
    sudo ip link delete br${i}
    sudo rm -rf /tmp/firecracker${i}.socket
    sudo rm -rf ../../assets/vms/system${i}
done





