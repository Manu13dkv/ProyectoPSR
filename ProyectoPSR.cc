#include "ns3/simulator.h"

#include "ns3/node-container.h"

#include "ns3/command-line.h"
#include "ns3/core-module.h"

#include "servidores.h"

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

//#define FACTORI 75
//#define FACTORG 75
//#define PROGRAMADOR 1
//#define COMERCIAL 1
//#define ADMIN 1
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ProyectoPsr"); 

int main (int argc, char *argv[])
{

Time::SetResolution(Time::NS);
//int factorglobal = FACTORG ;
//int factorindi = FACTORI;
//int n_programador= PROGRAMADOR;
//int n_comercial= COMERCIAL;
//int n_admin= ADMIN;
int num_clientes = 5;
Time duracion_simulacion = Time("100ms");

CommandLine cmd;

/*cmd.AddValue ("factorindi", "Factor de ocupación que tiene que cumplir cada servidor", factorindi);
cmd.AddValue ("programadores", "Numero de programadores que va a tener nuestro escenario", n_programador);
cmd.AddValue ("comerciales", "Numero de comerciales que va a tener nuestro escenario", n_comercial);
cmd.AddValue ("administradores", "Numero de administradores que va a tener nuestro escenario", n_admin);
*/

cmd.AddValue ("num_clientes", "Numero de clientes del sistema", num_clientes);

cmd.Parse (argc, argv);

Servidores *servidores = new Servidores();
servidores->GetNodos(num_clientes);

//poner para calcular el factor de ocuapcion global

 // Lanzamos simulación
NS_LOG_INFO("Arranca simulación");
Simulator::Stop(duracion_simulacion);
Simulator::Run();
NS_LOG_INFO("Fin simulación");


}