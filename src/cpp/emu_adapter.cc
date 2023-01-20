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
#include "emu_adapter.h"

// ns3 libraries.
//=========================================

// Standard libraries.
//=========================================


NS_LOG_COMPONENT_DEFINE("EmuAdapter");

using namespace ns3;

NS_OBJECT_ENSURE_REGISTERED (EmuAdapter);


EmuAdapter:: 
EmuAdapter(void){

    m_node = CreateObject<Node>();

    InternetStackHelper internetStackHelper;
    internetStackHelper.Install (m_node);
}


TypeId
EmuAdapter::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::EmuAdapter")
    .SetParent<Object>()
    .SetGroupName("Network")
    .AddConstructor<EmuAdapter>()
    ;
  return tid;
}


Ptr<Node>
EmuAdapter::GetNode(void){
    return m_node; 
}


std::string
EmuAdapter::GetTapName(void){
    return m_name; 
}


Ptr<NetDevice>
EmuAdapter::GetIface(void){
    return m_iface; 
}


Ptr<NetDevice>
EmuAdapter::AttachTapDevice(std::string tap_name, Ptr<CsmaNetDevice> iface){
    
    TapBridgeHelper tap_helper;
    tap_helper.SetAttribute("Mode", StringValue("UseLocal"));
    tap_helper.SetAttribute("DeviceName", StringValue(tap_name));
    tap_helper.Install(m_node, iface);

    m_iface = iface;
    
    return iface;
}










