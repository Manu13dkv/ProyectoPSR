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
//#include "ipv4router.h"
//#include "emu_adapter.h"

// ns3 libraries.
//=========================================
#include "ns3/core-module.h"
#include "ns3/ptr.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-channel.h"
#include "ns3/csma-module.h"

#include "ns3/abort.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/fd-net-device-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/v4ping.h"

#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/tap-bridge-module.h"


// Standard libraries.
//=========================================


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Main");

int main(int argc, char* argv[]){

    GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
    GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

    // Client side     

    Ptr<Node> guest = CreateObject<Node>();
    Ptr<Node> server = CreateObject<Node>();

    CsmaHelper csma;
    NetDeviceContainer devices = csma.Install(NodeContainer(
        guest,
        server
    ));

    InternetStackHelper stack;
    stack.Install (guest);
    stack.Install (server);

    Ptr<Ipv4> ipv41 = guest->GetObject<Ipv4>();
    uint32_t ip_index1 = ipv41->AddInterface(devices.Get(0));

    ipv41->  
        AddAddress(
          ip_index1,
          Ipv4InterfaceAddress(
            Ipv4Address("10.1.1.2"), 
            Ipv4Mask("255.255.255.0")
            )
        );

    ipv41->SetUp(ip_index1);

    ipv41->  
        SetForwarding(   
            ip_index1,
            true
        );

    Ptr<Ipv4> ipv42 = server->GetObject<Ipv4>();
    uint32_t ip_index2 = ipv42->AddInterface(devices.Get(1));

    ipv42->  
        AddAddress(
          ip_index2,
          Ipv4InterfaceAddress(
            Ipv4Address("10.1.1.1"), 
            Ipv4Mask("255.255.255.0")
            )
        );


    ipv42->SetUp(ip_index2);

    ipv42->  
        SetForwarding(   
            ip_index2,
            true
        );

    Ptr<Node> other = CreateObject<Node>();

    NetDeviceContainer odevices = csma.Install(NodeContainer(
        server,
        other
    ));

    stack.Install (other);

    uint32_t ip_index3 = ipv42->AddInterface(odevices.Get(0));

    ipv42->  
        AddAddress(
          ip_index3,
          Ipv4InterfaceAddress(
            Ipv4Address("10.1.2.2"), 
            Ipv4Mask("255.255.255.0")
            )
        );

    ipv42->SetUp(ip_index3);

    ipv42->  
        SetForwarding(   
            ip_index3,
            true
        );

    
    Ptr<Ipv4> ipv43 = other->GetObject<Ipv4>();
    uint32_t ip_index4 = ipv43->AddInterface(odevices.Get(1));

    ipv43->  
        AddAddress(
          ip_index4,
          Ipv4InterfaceAddress(
            Ipv4Address("10.1.2.1"), 
            Ipv4Mask("255.255.255.0")
            )
        );

    ipv43->SetUp(ip_index4);

    ipv43->  
        SetForwarding(   
            ip_index4,
            true
        );
    

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    TapBridgeHelper tapBridge;
    tapBridge.SetAttribute ("Mode", StringValue ("UseLocal"));
    tapBridge.SetAttribute ("DeviceName", StringValue ("tap1"));
    tapBridge.Install (guest, devices.Get(0));


    NS_LOG_INFO ("Run Emulation.");
    Simulator::Stop (Seconds (300.0));
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");
    
}
