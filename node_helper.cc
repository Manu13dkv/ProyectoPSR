#include "node_helper.h"
#include "ns3/log.h"
#include "ns3/csma-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NodeHelper");

NodeHelper::
NodeHelper(void)
{
   //TODO
}

NodeContainer
NodeHelper::GetNodos(int num_nodos, int tipo_nodo, char* red){

    NodeContainer c_nodos;

    c_nodos.Create (num_nodos);
    InternetStackHelper h_pila;
    h_pila.SetIpv6StackInstall(false);
    h_pila.Install(c_nodos);

    // Capa CSMA --> Instalamos CSMA a todos los nodos
    //=======================================================================================
    CsmaHelper h_csma;

    //h_csma.SetChannelAttribute ("DataRate", DataRateValue (RegBin));

    NetDeviceContainer c_dispositivos = h_csma.Install (c_nodos);

    // Capa ip -->
    //=======================================================================================
    char * mask = "255.255.255.0";
    Ipv4AddressHelper h_direcciones(red,mask); 

    switch(tipo_nodo){

        case CLIENTE:
            //FOO
        break;

        case SERVIDOR:
            h_direcciones.SetBase(red,mask,"0.0.0.250");
        break;
    }
    Ipv4InterfaceContainer c_interfaces = h_direcciones.Assign(c_dispositivos);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Logs
    //=======================================================================================

    for(int i = 0; i < num_nodos; i++){
        NS_LOG_INFO("ID nodo: " << c_nodos.Get(i)->GetId());
        NS_LOG_INFO("Dirección Mac: " << c_dispositivos.Get(i)->GetAddress());
    }

    return c_nodos;
}

