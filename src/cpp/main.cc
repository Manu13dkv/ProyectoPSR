/*
    Planificación y simulación de redes 2022/2023

    Authors:
        - Manuel Domínguez Montero.
        - Juan Manuel Alcalá Palomo.
        - Ismael García Mayorga.
        - Julio Navarro Vázquez

*/

// Own libraries
//=========================================
#include "ipv4router.h"
#include "emu_adapter.h"

// ns3 libraries.
//=========================================
#include "ns3/core-module.h"
#include "ns3/ptr.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"

#include "ns3/core-module.h"
#include "ns3/ptr.h"
#include "ns3/net-device-container.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-helper.h"
#include "ns3/csma-channel.h"
#include "ns3/data-rate.h"
#include "ns3/ipv4-address.h"
#include "ns3/node.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/ipv4-address-helper.h"

#include "ns3/command-line.h"

// Standard libraries.
//=========================================


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Main");

int main(int argc, char* argv[]){


    GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
    GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

    // Default Values.
    //
    // *_r (Raw) sufix to advise that the variable will be wrapped 
    // by an ns3 object, such as Time object to a double.
    //  
    //================================================================================
    std::string sim_len_r = "3600s";
    uint32_t n_nodes = 2;
    std::string base_ip_r = "10.1.1.0";
    std::string base_mask_r = "255.255.255.0";
    //================================================================================

    CommandLine cmd(__FILE__);

    cmd.AddValue("sim_len", "Length of the simulation.", sim_len_r);
    cmd.AddValue("n_nodes", "Number of real nodes to insert in the simulation.", n_nodes);

    cmd.Parse(argc, argv);

    // Wrapping parameters.
    //
    //================================================================================
    Time sim_len = Time(sim_len_r);
    Ipv4Address base_ip = Ipv4Address(base_ip_r.c_str());
    Ipv4Mask base_mask = Ipv4Mask(base_mask_r.c_str());
    //================================================================================

    // Configuring scenario.
    //================================================================================

    Ipv4AddressHelper ip_helper;
    ip_helper.SetBase(base_ip, base_mask);

    Ptr<Ipv4Router> router = CreateObject<Ipv4Router>();
    uint32_t tap_index = 1;

    for ( uint32_t node = 0; node < n_nodes; node++){

        router->IfUp(       
            "eth"+std::to_string(node),    // --> eth0, eth1 ... 
            ip_helper.NewAddress(),
            base_mask
        );
     
        Ptr<EmuAdapter> tap = CreateObject<EmuAdapter>();

        Ptr<NetDevice> guest_iface = router->Link(
            tap->GetNode(),
            "eth"+std::to_string(node),
            ip_helper.NewAddress(),
            base_mask
        );

        // When ns3 creates a tap device, then our system links it to a bridge 
        // and this to another tap device, hence if ns3 create tap1, our system will 
        // generate tap 2. Thus, the next tap in ns3 will be directly tap3. 
        tap->AttachTapDevice("tap"+std::to_string(tap_index), guest_iface);
        tap_index+=2;
        ip_helper.NewNetwork();
    }
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    NS_LOG_INFO ("Run Emulation.");
    Simulator::Stop (sim_len);
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");
    
}
