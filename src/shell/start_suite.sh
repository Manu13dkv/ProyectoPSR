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
num_taps=2
base_ip="10.1.1.2"

# Display usage message
usage() {
    echo "Usage: $0 [-n <number of tap devices>] [-b base ip for tap devices]"
}
	
# Parse command-line arguments
while [[ $# -gt 0 ]]
do
    key="$1"

    case $key in
        -n|--num_taps)
            num_taps="$2"
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

# Setting up virtual network.
#
# The ip address configured here will be deleted and reasigned eventually.
# However, it is neccesary in order to "bind" the tap devices to the ns3 nodes. 
#
#====================================================================================

ip=${base_ip}

echo "Setting up virtual bridges and tap devices..."

for i in $(seq 1 $num_taps)
do
    tap_name=tap${i}
    bridge_name=br${i}

    sudo ip link add name ${bridge_name} type bridge
    sudo ip link set dev ${bridge_name} up

    sudo ip tuntap add dev ${tap_name}f mode tap
    sudo ifconfig ${tap_name}f hw ether 00.00.00.00.00.1${i}

    sudo ip tuntap add dev ${tap_name}ns mode tap
    sudo ifconfig ${tap_name}ns hw ether 00.00.00.00.00.0${i}

    sudo brctl addif ${bridge_name} ${tap_name}f
    sudo brctl addif ${bridge_name} ${tap_name}ns

    sudo ifconfig ${tap_name}f up
    sudo ifconfig ${tap_name}ns up

    sudo sysctl -w net.ipv4.ip_forward=1
    sudo sysctl -w net.ipv4.conf.${tap_name}f.proxy_arp=1
    sudo sysctl -w net.ipv6.conf.${tap_name}f.disable_ipv6=1
    sudo sysctl -w net.ipv4.conf.${tap_name}ns.proxy_arp=1
    sudo sysctl -w net.ipv6.conf.${tap_name}ns.disable_ipv6=1

    sudo ifconfig ${tap_name} hw ether 00.00.00.00.00.0${i}
    sudo ifconfig ${tap_name} 0.0.0.0 up
    sudo brctl addif ${bridge_name} ${tap_name} 

done

echo "Done."

# Setting up the Firecracker's micro virtual machines
#
#====================================================================================

echo "Setting up the Firecracker uV.Machines..."

for i in $(seq 1 $num_taps)
do
    echo "Setting Up the kernel and filesystem for the uvm ${i}..."

    tap_name=tap${i}
    system_path=../../assets/vms/system${i}
    kernel_path=${system_path}/vmlinux
    fs_path=${system_path}/cent.ext4

    cp -r ${HOME}/vms/firecracker/CentOS-8-virgin/CentOS-8/system ${system_path}

    echo "Done."

    echo "Launching uvm ${i}"

    xterm -e bash -c "firecracker --api-sock /tmp/firecracker${i}.socket; exec bash"&
    sleep 2
    xterm -e bash -c "~/test/firecracker/start_vm.sh -si ${i} -fs ${fs_path} -k ${kernel_path} -tn ${tap_name}f; exec bash"&
    sleep 10

    echo "done"

done


# Configuring ns3 scenario.
#
#====================================================================================

echo "Configuring Ns3 Sscenario..."

xterm -e bash -c "NS_LOG='Main:info' ${HOME}/.local/bin/ns3/ns-3.36.1/ns3 run main.cc; exec bash"&
sleep 5

echo "Done."


# Moving the tap devices to isolated network namespaces.
#
# It has to be done after building the ns3 scenario.
#
#====================================================================================

#echo "Creating new network namespaces."

#ip=${base_ip}

#for i in $(seq 1 $num_taps)
#do
#    tap_name=tap${i}
#    tap_namespace=${tap_name}ns

#    sudo ip netns add ${tap_namespace}
#    sudo ip link set dev ${tap_name} netns ${tap_namespace}
#    sudo ip netns exec ${tap_namespace} ip link set dev ${tap_name} up
#    sudo ip netns exec ${tap_namespace} ip address add ${ip}/24 dev ${tap_name}

#    IFS="." read -a a <<< ${ip}
#    IFS="." read -a b <<< 0.0.1.0  #Adding a subnet for each tap device
#    ip="$[a[0]+b[0]].$[a[1]+b[1]].$[a[2]+b[2]].$[a[3]+b[3]]"
#
#done

#echo "Done"

