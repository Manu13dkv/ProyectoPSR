#!/bin/bash
#
# Configuration and Starting of a Firecracker virtual Machine
#
# Author: Manuel Domínguez Montero.
#
# 2022/2023

# Default values
tap_name="tap0"
tap_addr="00:00:00:00:01:01"
#tap_ip="172.17.0.1/24"
if_name="eth0"
gw_name="wlan0"
rootfs_path="${HOME}/vms/firecracker/CentOS-8/system/cent.ext4"
kernel_path="${HOME}/vms/firecracker/CentOS-8/system/vmlinux"
arch=`uname -m`
vcpus=2
mem_size=1024

# Display usage message
usage() {
    echo "Usage: $0 [-fs <Filesystem path>] [-k <Kernel Path>] [-p <Number of V.Processors>] [-m <RAM memory size>] [-tn <tap device name>] [-tip <tap device ip>]  [-si socket identifier]"
}
	
# Parse command-line arguments
while [[ $# -gt 0 ]]
do
    key="$1"

    case $key in
        -fs|--rootfs_path)
            rootfs_path="$2"
            shift # past argument
            shift # past value
            ;;
        -k|--kernel_path)
            kernel_path="$2"
            shift # past argument
            shift # past value
            ;;
        -p|--vcpus)
            vcpus="$2"
            shift # past argument
			shift # past value
            ;;
		-m|--mem_size)
            mem_size="$2"
            shift # past argument
 			shift # past value
            ;;
        -tn|--tap_name)
            tap_name="$2"
            shift # past argument
            shift # past value
            ;;
        -ta|--tap_addr)
            tap_addr="$2"
            shift # past argument
            shift # past value
            ;;
        -tip|--tap_ip)
            tap_ip=$"2"
            shift # past argument
			shift # past value
            ;;
		-gw|--gw_name)
            gw_name=$"2"
            shift # past argument
 			shift # past value
            ;;
        -si|--socket_identifier)
            socket_identifier="$2"
            shift # past argument
 			shift # past value
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            # Unknown option
            shift # past argument
            ;;
    esac
done

# Kernel configuration.
#====================================================================================

if [ ${arch} = "x86_64" ]; then
    curl --unix-socket /tmp/firecracker${socket_identifier}.socket -i \
      -X PUT 'http://localhost/boot-source'   \
      -H 'Accept: application/json'           \
      -H 'Content-Type: application/json'     \
      -d "{
            \"kernel_image_path\": \"${kernel_path}\",
            \"boot_args\": \"console=ttyS0 reboot=k panic=1 pci=off\"
       }"
elif [ ${arch} = "aarch64" ]; then
    curl --unix-socket /tmp/firecracker${socket_identifier}.socket -i \
      -X PUT 'http://localhost/boot-source'   \
      -H 'Accept: application/json'           \
      -H 'Content-Type: application/json'     \
      -d "{
            \"kernel_image_path\": \"${kernel_path}\",
            \"boot_args\": \"keep_bootcon console=ttyS0 reboot=k panic=1 pci=off\"
       }"
else
    echo "Cannot run firecracker on $arch architecture!"
    exit 1
fi

echo "Kernel settled up"

# Host Network configuration
#================================================================
#echo "Host Network configuration..."
#echo "Saving Netfilter rules to iptables.rules.old"
#sudo iptables-save > iptables.rules.old


#sudo ip tuntap add dev ${tap_name} mode tap
#sudo sysctl -w net.ipv4.conf.tap0.proxy_arp=1
#sudo sysctl -w net.ipv6.conf.tap0.disable_ipv6=1
#sudo ip addr add ${tap_ip} dev ${tap_name}
#sudo ip link set ${tap_name} up

#sudo iptables -t nat -A POSTROUTING -o ${gw_name} -j MASQUERADE
#sudo iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
#sudo iptables -A FORWARD -i ${tap_name} -o ${gw_name} -j ACCEPT

#echo "Gateway established in ${tap_name} with ip: ${tap_ip}"
#echo "Previous Netfilter rules saved to iptables.rules.old"
#echo "network configuration done"

# FileSystem Configuration.
#====================================================================================
curl --unix-socket /tmp/firecracker${socket_identifier}.socket -i \
    -X PUT 'http://localhost/drives/rootfs' \
    -H 'Accept: application/json'           \
    -H 'Content-Type: application/json'     \
    -d "{
            \"drive_id\": \"rootfs\",
            \"path_on_host\": \"${rootfs_path}\",
            \"is_root_device\": true,
            \"is_read_only\": false
    }"

echo "File system settled up"

# Virtual Hardware configuration.
#===================================================================================
curl --unix-socket /tmp/firecracker${socket_identifier}.socket -i  \
    -X PUT 'http://localhost/machine-config' \
    -H 'Accept: application/json'            \
    -H 'Content-Type: application/json'      \
    -d "{
      \"vcpu_count\": ${vcpus},
      \"mem_size_mib\": ${mem_size}
    }"

echo "Hardware config done"

# Guest network configuration.
#===================================================================================
curl --unix-socket /tmp/firecracker${socket_identifier}.socket \
    -X PUT "http://localhost/network-interfaces/${if_name}" \
    -H 'accept:application/json' \
    -H 'Content-Type:application/json' \
    -d "{
        \"iface_id\": \"${if_name}\",
        \"guest_mac\": \"${tap_addr}\",
        \"host_dev_name\": \"${tap_name}\"
    }"
               
echo "Guest network up"

# Calling InstanceStart, vm initialization.
#===================================================================================
curl --unix-socket /tmp/firecracker${socket_identifier}.socket -i \
  -X PUT 'http://localhost/actions'       \
  -H  'Accept: application/json'          \
  -H  'Content-Type: application/json'    \
  -d '{
      "action_type": "InstanceStart"
    }'

echo "called InstanceStart"




