/*
    Planificación y simulación de redes 2022/2023
    Autores:
        - Manuel Domínguez Montero.
        - Juan Manuel Alcalá Palomo.
        - Ismael García Mayorga.
        - Julio Navarro Vázquez
*/

// own libraries.
//=========================================

// ns3 libraries.
//=========================================
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

// standard libraries.
//=========================================


using namespace ns3;

class Ipv4Router : public Object
{
public:
    Ipv4Router();

    static TypeId GetTypeId (void);

    NetDeviceContainer      GetIfaces();
    Ptr<CsmaNetDevice>      GetIface(std::string iface_name);      
    Ptr<CsmaNetDevice>      IfUp(std::string iface_name, Ipv4Address ip, Ipv4Mask mask);
    Ptr<CsmaNetDevice>      Link(Ptr<Node> guest, Ipv4Address guest_ip, Ipv4Mask guest_mask, std::string iface_name);
    DataRate                GetDataRate(std::string iface_name);
    void                    SetDataRate(std::string iface_name, DataRate data_rate);

    //Accessors.
    void                    SetGlobalDataRate(DataRate data_rate);
    DataRate                GetGlobalDataRate();

private:
    
    Ptr<Node> m_node;
    NetDeviceContainer m_ifaces;
    DataRate m_gdata_rate;
    
};