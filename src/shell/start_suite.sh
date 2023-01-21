#!/bin/bash
#
#    Planificación y simulación de redes 2022/2023
#
#    Authors:
#        - Manuel Domínguez Montero.
#        - Juan Manuel Alcalá Palomo.
#        - Ismael García Mayorga.
#        - Julio Navarro Vázquez
#

# Default values
n_nodes=2
base_ip="10.1.1.2"

# Display usage message
usage() {
    echo "Usage: $0 [-n <number of virtual nodes>] [-b base ip for tap devices]"
}
	
# Parse command-line arguments
while [[ $# -gt 0 ]]
do
    key="$1"

    case $key in
        -n|--n_nodes)
            n_nodes="$2"
            shift # past argument
            shift # past value
            ;;
		-b|--base_ip)
            base_ip=$"2"
            shift # past argument
 			shift # past value
            ;;
        -h|--help)
            gw_name=true
            usage
            exit 0
            ;;
        *)
            # Unknown option
            shift # past argument
            ;;
    esac
done

# Configuring ns3 scenario.
#
#====================================================================================

echo "Configuring Ns3 Scenario..."

xterm -e bash -c "NS_LOG='Main:info' ${HOME}/.local/bin/ns3/ns-3.36.1/ns3 run 'main.cc --n_nodes=${n_nodes}'; exec bash"&
sleep 10

echo "Done."

# Configuring  Linux virtual devices.
#====================================================================================
n_taps=$(( n_nodes * 2 ))

#ip=${base_ip}

#echo "Setting up virtual bridges and tap devices..."

tap_index=2
while [ $tap_index -le $n_taps ]
do
    sudo ip tuntap add dev tap${tap_index} mode tap    # This is the tap to be attached to the VM.
    sudo ip link set dev tap${tap_index} up

    tap_index=$(( tap_index + 2 ))  
done

tap_index=1
while [ $tap_index -le $n_taps ]
do
    sudo ip link set dev tap${tap_index} up  #Previous tap is created by ns3.
    tap_index=$(( tap_index + 2 ))
done

tap_index=1
for i in $(seq 1 $n_nodes)
do
    sudo brctl addbr br${i}
    sudo ip link set dev br${i} up

    sudo brctl addif br${i} tap${tap_index}

    tap_index=$(( tap_index + 1 ))

    sudo brctl addif br${i} tap${tap_index}
    
    tap_index=$(( tap_index + 1 ))
    
done


for tap_index in $(seq 1 $n_taps)
do
    sudo sysctl -w net.ipv4.conf.tap${tap_index}.proxy_arp=1  
    sudo sysctl -w net.ipv6.conf.tap${tap_index}.disable_ipv6=1
done

sudo sysctl -w net.ipv4.ip_forward=1

echo "Done."

# Setting up the Firecracker's micro virtual machines
#
#====================================================================================

echo "Setting up the Firecracker uV.Machines..."

tap_index=2
for node in $(seq 1 $n_nodes)
do
    echo "Setting Up the kernel and filesystem for the uvm ${node}..."
    
    tap_name=tap${tap_index}
    tap_addr=00:00:00:01:00:0${node}
    system_path=../../assets/vms/system${node}
    kernel_path=${system_path}/vmlinux
    fs_path=${system_path}/cent.ext4

    cp -r ${HOME}/vms/firecracker/CentOS-8-virgin/CentOS-8/system ${system_path}

    echo "Done."

    echo "Launching uvm ${node}"

    xterm -e bash -c "firecracker --api-sock /tmp/firecracker${node}.socket; exec bash"&
    sleep 4
    . ${HOME}/test/firecracker/start_vm.sh -si ${node} -fs ${fs_path} -k ${kernel_path} -tn ${tap_name} -ta ${tap_addr}&
    sleep 10

    tap_index=$(( tap_index + 2 ))

    echo "done"

done


# Moving the tap devices to isolated network namespaces.
#
# It has to be done after building the ns3 scenario.
#
#====================================================================================

#echo "Creating new network namespaces."

#ip=${base_ip}

#for i in $(seq 1 $n_nodes)
#do
#    bridge_name=br${i}
#    tap_name=tap${i}
#    
#    namespace=${tap_name}ns
#
#    ip link add name ${bridge_name} type bridge
#
#    ip link add
#

#    sudo ip netns add ${namespace}
#    sudo ip link set dev ${tap_name}ns netns ${tap_namespace}
#    sudo ip link set dev ${tap_name}f netns ${tap_namespace}

#    sudo ip netns exec ${namespace} ip link set dev ${tap_name}ns up
#    sudo ip netns exec ${namespace} ip link set dev ${tap_name}ns up

#    sudo ip netns exec ${namespace} ip address add ${ip}/24 dev ${bridge_name}

#    IFS="." read -a a <<< ${ip}
#    IFS="." read -a b <<< 0.0.1.0  #Adding a subnet for each tap device
#    ip="$[a[0]+b[0]].$[a[1]+b[1]].$[a[2]+b[2]].$[a[3]+b[3]]"

#done

#echo "Done"


echo "Sandboxing Suite"
echo "==================================================="
echo "Press 'q' to quit."

while true; do
  read -n 1 input
  if [ "$input" == "q" ]; then
    . delete_scenario.sh $n_nodes
    exit 0
  fi
done
