#include "servidores.h"
#include "ns3/log.h"
#include "ns3/csma-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Servidores");

Servidores::
Servidores(void)
{
   //TODO
}

NodeContainer
Servidores::GetNodos(int num_servidores){

    NodeContainer c_servidores;

    //creamos nodos

    c_servidores.Create (num_servidores);
    InternetStackHelper h_pila;
    h_pila.SetIpv6StackInstall(false);
    h_pila.Install(c_servidores);

    // Capa CSMA --> Instalamos ppp a todos los nodos
    CsmaHelper h_csma;

    //h_csma.SetChannelAttribute ("DataRate", DataRateValue (RegBin));

    NetDeviceContainer c_red_servidores = h_csma.Install (c_servidores);

    // Capa ip -->
    Ipv4AddressHelper h_direcciones("10.1.10.0", "255.255.255.0");
    Ipv4InterfaceContainer c_interfaces = h_direcciones.Assign(c_red_servidores);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Capa de transporte.

    //-->UDP
    UdpServerHelper h_Server(20);
    
    h_Server.Install(c_servidores);

    // Sumario de nodos servidores.

    UintegerValue port_servidor;

    /*for(int i = 0; i < num_servidores; i++){
        NS_LOG_INFO("ID nodo servidor: " << c_servidores.Get(i)->GetId());
        NS_LOG_INFO("Dirección Mac: " << c_red_servidores.Get(i)->GetAddress());
        NS_LOG_INFO("Puerto escucha Servidor UDP: " << c_serverApps.Get(0)->GetObject<UdpServer>()->GetAttribute("Port", port_servidor));
    }*/
    return c_servidores;
}

