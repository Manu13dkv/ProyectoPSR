#!/bin/bash
#
# Planificación y simulación de redes 2022/2023
# 
# Manuel Domínguez Montero.
#

# Virtual machines shutdown.
#
#==============================================================

echo "shutting down virtual machines."

#(ssh dit@10.1.1.2 sudo reboot)&
#(ssh dit@10.1.2.2 sudo reboot)&
#(ssh dit@10.1.3.2 sudo reboot)&

# Delete all virtual net devices.
# Assumming that all identifiers are between 0 to 10
#==================================================================
echo "Deleting tap devices and bridges..."

mapfile -t taps < <( ip a | awk -F ":" '{print $2}' | grep tapns )
mapfile -t sockets < <( ls /tmp/ | grep firecracker* ) 
mapfile -t bridges < <( ip a | awk -F ":" '{print $2}' | grep brns )

for tap in "${taps[@]}"
do
    sudo ip link delete dev $tap
done

for bridge in "${bridges[@]}"
do
    sudo ip link delete $bridge
done

echo "Deleting firecracker Sockets..."

for socket in "${sockets[@]}"
do
    rm -rf /tmp/${socket}
done





