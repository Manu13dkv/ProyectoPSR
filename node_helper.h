/*
Esta clase nos proporciona un contenedor de nodos ya configurado.
*/

#include "ns3/node-container.h"
#include "ns3/udp-server.h"
#include "ns3/core-module.h"
#include "ns3/simulator.h"
#include "ns3/ptr.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/application-container.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/log.h"
#include "ns3/csma-helper.h"

//Librerias propias
//=======================================================================================

//Librerias del standard de C++
#include <iostream>
#include <string>


//=======================================================================================

//Constantes
#define SERVIDOR 0
#define CLIENTE 1

//=======================================================================================

using namespace ns3;

class NodeHelper
{
public:
    NodeHelper      (void);
    NodeContainer GetNodos(int num_nodos, int tipo_nodo, char* red);

private:
};

