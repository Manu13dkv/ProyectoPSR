#include "ns3/simulator.h"

#include "ns3/node-container.h"

#include "ns3/command-line.h"
#include "ns3/core-module.h"

#include "ns3/bridge-helper.h"


//Librerias propias
//=======================================================================================
#include "node_helper.h"

//=======================================================================================

/*#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/node.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/udp-server.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/data-rate.h"
#include "ns3/channel.h"

#include <string>

#include "observador.h"
#include "retardo.h"
*/

//Constantes
#define SERVIDOR 0
#define CLIENTE 1
//=======================================================================================


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ProyectoPSR"); 

int main (int argc, char *argv[])
{

Time::SetResolution(Time::NS);

// Parametros por defecto
//=======================================================================================
int num_clientes = 5;
int num_servidores = 1;
Time duracion_simulacion = Time("100ms");

// CMD 
//=======================================================================================
CommandLine cmd;

cmd.AddValue ("num_clientes", "Numero de clientes del sistema", num_clientes);

cmd.Parse (argc, argv);

//Configuración del escenario
//=======================================================================================
NodeContainer c_servidores;
NodeContainer c_clientes;
NodeContainer c_nodos;

NodeHelper* h_nodos = new NodeHelper(); //Nos configura hasta nivel IP cualquier nodo

//Asignamiento de direcciones, clientes más baja, servidores, mas alta.
c_servidores = h_nodos->GetNodos(num_servidores, SERVIDOR, "10.10.10.0");
c_clientes = h_nodos->GetNodos(num_clientes, CLIENTE, "10.10.10.0");

c_nodos.Add(c_clientes);
c_nodos.Add(c_servidores);

//Capa de aplicación.
//=======================================================================================

//Sistema cliente-servidor UDP básico que nos permite obtener parámetros de GoS iniciales.
uint16_t puerto_servidor =20;

UdpServerHelper h_servers_apps(puerto_servidor);    
h_servers_apps.Install(c_servidores);

UdpClientHelper h_clients_apps(c_servidores.Get(0)->GetObject<Ipv4>()->GetAddress(0,0).GetLocal(), puerto_servidor);
h_clients_apps.Install(c_clientes);

//Conexiones al Puente
BridgeHelper h_bridge;
Ptr<Node> puente = CreateObject<Node>();
NetDeviceContainer c_dispositivos;
NetDeviceContainer c_disp_bridge;

for (int i = 0; i < (num_clientes + num_servidores); i++ ){
      c_disp_bridge.Add(c_nodos.Get(i)->GetDevice(1));
}
h_bridge.Install (puente, c_disp_bridge);

//=======================================================================================
// Simulación
//=======================================================================================
NS_LOG_INFO("Arranca simulación");
Simulator::Stop(duracion_simulacion);
Simulator::Run();
NS_LOG_INFO("Fin simulación");
//=======================================================================================

}