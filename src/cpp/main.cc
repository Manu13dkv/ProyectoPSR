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

// Standard libraries.
//=========================================


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Main");

int main(int argc, char* argv[]){

    GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
    GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

    // Client side     
    Ptr<EmuAdapter> tap1 = CreateObject<EmuAdapter>();

    // Server side
    Ptr<EmuAdapter> tap2 = CreateObject<EmuAdapter>();

    Ptr<Node> prueba = CreateObject<Node>();
    InternetStackHelper stack;
    stack.Install(prueba);
   
    // Router
    Ptr<Ipv4Router> router = CreateObject<Ipv4Router>();

    router->IfUp(
        "eth0",
        Ipv4Address("10.1.1.1"),
        Ipv4Mask("255.255.255.0")
    );

    router->IfUp(
        "eth1",
        Ipv4Address("10.1.2.1"),
        Ipv4Mask("255.255.255.0")
    );
   
    router->IfUp(
        "eth2",
        Ipv4Address("10.1.3.1"),
        Ipv4Mask("255.255.255.0")
    );

    Ptr<CsmaNetDevice> guest_iface = router->Link(
        tap1->GetNode(),
        Ipv4Address("10.1.1.2"),
        Ipv4Mask("255.255.255.0"),
        "eth0"
    );

    Ptr<CsmaNetDevice> server_iface = router->Link(
        tap2->GetNode(),
        Ipv4Address("10.1.2.2"),
        Ipv4Mask("255.255.255.0"),
        "eth1"
    );

    router->Link(
        prueba,
        Ipv4Address("10.1.3.2"),
        Ipv4Mask("255.255.255.0"),
        "eth2"
    );

    tap1->AttachTapDevice("tap1", guest_iface);
    tap2->AttachTapDevice("tap2", server_iface);
   
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    NS_LOG_INFO ("Run Emulation.");
    Simulator::Stop (Seconds (3600.0));
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");
    
}
