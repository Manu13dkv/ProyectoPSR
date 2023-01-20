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

// ns3 libraries.
//=========================================
#include "ns3/abort.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/fd-net-device-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/csma-net-device.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/csma-helper.h"


// Standard libraries.
//=========================================

// Constants.
//=========================================


using namespace ns3;

class EmuAdapter : public Object
{
public:

    EmuAdapter(void);

    static TypeId GetTypeId (void);

    Ptr<Node>       GetNode(void);
    std::string     GetTapName(void);
    Ptr<NetDevice>  GetIface(void);
    Ptr<NetDevice>  AttachTapDevice(std::string tap_name, Ptr<CsmaNetDevice> iface);

private:

    std::string m_name;
    Ptr<Node> m_node;
    Ptr<NetDevice> m_iface;
    
};