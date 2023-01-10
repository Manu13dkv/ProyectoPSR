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

// ns3 libraries.
//=========================================
#include "ns3/core-module.h"
#include "ns3/simulator.h"
#include "ns3/command-line.h"

#include "ns3/internet-stack-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/node.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/application-container.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/csma-net-device.h"

// Standard libraries.
//=========================================


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Main");


int main(int argc, char* argv[]){

    //Tests

    Ptr<Node> guest = CreateObject<Node>();

    Ptr<Ipv4Router> router = new Ipv4Router();

    router->IfUp(
        "eth0",
        Ipv4Address("10.10.10.1"),
        Ipv4Mask("255.255.255.0")
    );

    router->Link(guest, "eth0");

    NS_LOG_INFO("Number of Interfaces: " << router->GetIfaces().GetN());

    NS_LOG_INFO(
        router->GetDataRate("eth0")
    );

    NS_LOG_INFO(
        router->GetGlobalDataRate()
    );

    NS_LOG_INFO(
        guest->GetDevice(0)->GetChannel()->GetObject<CsmaChannel>()->GetDataRate()
    );

    router->SetDataRate("eth0", DataRate("32Kbps"));

    router->SetGlobalDataRate(DataRate("10bps"));

    NS_LOG_INFO(router->GetDataRate("eth0"));

    NS_LOG_INFO("Simulation starts.");
    Simulator::Stop();
    Simulator::Run();
    NS_LOG_INFO("Simulation ends.");
    
}
