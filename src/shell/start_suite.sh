#!/bin/bash
#
#    Planificación y simulación de redes 2022/2023
#
#    Authors:
#        - Manuel Domínguez Montero.
#        - Juan Manuel Alcalá Palomo.
#        - Ismael García Mayorga.
#        - Julio Navarro Vázquez
#=====================================================================================

# Default values
n_nodes=1
base_ip="10.1.1.2"
init_script=init_script.sh
sim_len=3600s
text_dir="../../assets/text/"
image_dir="../../assets/images/"
pdf_file="../../assets/simulation.pdf"

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
        -t|--sim_len)
            sim_len=$"2"
            shift # past argument
 			shift # past value
            ;;
        -b|--base_ip)
            base_ip=$"2"
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

# Ns3 scenario.
#
#====================================================================================


echo "Configuring Ns3 Scenario..."

$( NS_LOG='Main:info' ${HOME}/.local/bin/ns3/ns-3.36.1/ns3 run 'main.cc --n_nodes=1 --sim_len=60s' )&

ns3_pid=$!
sleep 20

echo "Done."

# Configuring  Linux virtual devices.
#====================================================================================
n_taps=$(( n_nodes * 2 ))

#ip=${base_ip}

echo "Setting up virtual bridges and tap devices..."

# Tap device created by ns3, to management the nodes from outside ns3. 
sudo ip link set dev tapns0 up
sudo ip addr add 10.1.10.2/24 dev tapns0
sudo ip route add 10.1.0.0/16 via 10.1.10.1 dev tapns0
sudo sysctl -w net.ipv4.conf.tapns0.proxy_arp=1  
sudo sysctl -w net.ipv6.conf.tapns0.disable_ipv6=1

echo "Management device is tapns0, ip address: 10.1.10.2/24"

#Other virtual devices, to give the VM hosts access to ns3 scenario. 
tap_index=2
while [ $tap_index -le $n_taps ]
do
    sudo ip tuntap add dev tapns${tap_index} mode tap    # This is the tap to be attached to the VM.
    sudo ip link set dev tapns${tap_index} up
    tap_index=$(( tap_index + 2 ))  
done

tap_index=1
while [ $tap_index -le $n_taps ]
do
    sudo ip link set dev tapns${tap_index} up  #Previous tap is created by ns3.
    tap_index=$(( tap_index + 2 ))
done

tap_index=1
for i in $(seq 1 $n_nodes)
do
    sudo brctl addbr brns${i}
    sudo ip link set dev brns${i} up
    sudo brctl addif brns${i} tapns${tap_index}
    tap_index=$(( tap_index + 1 ))
    sudo brctl addif brns${i} tapns${tap_index}
    tap_index=$(( tap_index + 1 ))
    
done


for tap_index in $(seq 1 $n_taps)
do
    sudo sysctl -w net.ipv4.conf.tapns${tap_index}.proxy_arp=1  
    sudo sysctl -w net.ipv6.conf.tapns${tap_index}.disable_ipv6=1
done

sudo sysctl -w net.ipv4.ip_forward=1

echo "Done."


# Setting up the Firecracker's micro virtual machines
#
#====================================================================================
echo "Virtual Network configured, Setting up Firecracker VMs."

tap_index=2
for node in $(seq 1 $n_nodes)
do
    echo "Launching uvm ${node}..."
    
    tap_name=tapns${tap_index}
    tap_addr=00:00:00:01:00:0${node}
    system_path=../../assets/vms/system${node}
    kernel_path=${system_path}/vmlinux
    fs_path=${system_path}/cent.ext4

    xterm -e bash -c "firecracker --api-sock /tmp/firecracker${node}.socket; exec bash"&
    sleep 5

    . start_vm.sh -si ${node} -fs ${fs_path} -k ${kernel_path} -tn ${tap_name} -ta ${tap_addr}&
    sleep 20

    tap_index=$(( tap_index + 2 ))

    echo "done"

done

echo "Running simulation..." 

# Init script 
# 
# In order to configure the cluster,
# use the management device to reach the end points.
#
# Management Tap: 
#
# name: tap0
# ip address: 10.1.10.2 
#
#================================================================
if [[ $init_script ]]
then
    echo "Launching init_script"
    ( . $init_script )&
fi

wait $ns3_pid

echo "Simulation finished, storing data inside assets directory..."
mv ${HOME}/.local/bin/ns3/ns-3.36.1/*pcap ../../assets/pcap/
echo "pcap files inside assets directory"

# Measuring http request-response delay.
#
#=========================================================================================
echo "Measuring Average http request-response delay."

mapfile -t pcap_files < <( ls ../../assets/pcap/ | grep pcap ) 
for pcap in "${pcap_files[@]}"
do
    # Use tshark to extract the time since request filed for HTTP responses
    tshark -r ../../assets/pcap/${pcap} -T fields -e frame.time_relative -e http.time -Y 'http.response' | awk '{sum+=$2; ++n} END {print sum/n}' > ../../assets/text/${pcap}.txt 
done

# Plots Bandwidth usage
#
# ========================================================================================
echo "Measuring Bandwidth Usage for each Pod of the clúster"

for pod in "${pcap_files[@]}"
do
    xterm -e bash -c ". get_bandwidth.sh ../../assets/pcap/${pod} 0.08"&
done 


# Building the pdf file
#
# ========================================================================================
echo "Generating PDF file."

#python3 ../python/pdf_generator.py --image_dir ${image_dir} --text_dir ${text_dir} --pdf_file ${pdf_file}

echo "PDF output file is: simulation.pdf"
#$(gio open ../../assets/simulation.pdf)&

echo "Data files will be erased on a new simulation, please, save them before quit."

echo "Process complete"

echo "Press q to quit."

# Expecting close signal
#
#===========================================
while true; do
  read -n 1 input
  if [ "$input" == "q" ]; then
    echo "Closing program..."
    . delete_scenario.sh
    exit 0
  fi
done


