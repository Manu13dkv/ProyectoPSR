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

using namespace ns3;

class Servidores
{
public:
    Servidores      (void);
    NodeContainer GetNodos(int num_servidores);

private:
};

