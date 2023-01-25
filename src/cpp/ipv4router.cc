/*
    Planificación y simulación de redes 2022/2023
    Autores:
        - Manuel Domínguez Montero.
        - Juan Manuel Alcalá Palomo.
        - Ismael García Mayorga.
        - Julio Navarro Vázquez
*/

// Own libraries.
//=========================================
#include "ipv4router.h"

// ns3 libraries.
//=========================================

// Standard libraries.
//=========================================


NS_LOG_COMPONENT_DEFINE("Ipv4Router");

using namespace ns3;

NS_OBJECT_ENSURE_REGISTERED (Ipv4Router);

Ipv4Router::
Ipv4Router(void)
{
  m_node = CreateObject<Node>();
  m_ifaces = NetDeviceContainer();
  m_gdata_rate = DataRate("100Mbps");
  
  InternetStackHelper h_inetstack;
  h_inetstack.Install(m_node);
  
}


TypeId
Ipv4Router::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::IPv4Router")
    .SetParent<Object>()
    .SetGroupName("Network")
    .AddConstructor<Ipv4Router>()
    ;
  return tid;
}


void 
Ipv4Router::SetDataRate(std::string iface_name, DataRate data_rate){
   
  Ptr<CsmaNetDevice> iface = GetIface(iface_name);

  if(iface==nullptr){
    NS_LOG_ERROR(
      "The interface "<<iface_name <<" does not exists, router id: "<< m_node->GetId()
    );
  }else{
    Ptr<CsmaChannel> channel = iface->GetChannel()->GetObject<CsmaChannel>();
    if( channel == nullptr){
      NS_LOG_ERROR(
      "The interface "<< iface_name <<" is disconnected to any channel, router id: "<< m_node->GetId()
      );
    }else{
      channel->SetAttribute("DataRate", DataRateValue(data_rate));
    }
  }
}


DataRate
Ipv4Router::GetDataRate(std::string iface_name){

  Ptr<CsmaNetDevice> iface = GetIface(iface_name);
  DataRate data_rate;

  if(iface==nullptr){
    NS_LOG_ERROR(
      "The interface "<<iface_name <<" does not exists, router id: "<< m_node->GetId()
    );
  }else{
    Ptr<CsmaChannel> channel = iface->GetChannel()->GetObject<CsmaChannel>();
    if(channel == nullptr){
      NS_LOG_ERROR(
      "The interface "<< iface_name <<" is disconnected to any channel, router id: "<< m_node->GetId()
      );
    }else{
      data_rate = channel->GetDataRate();
    }
  }

  return data_rate;
}

void
Ipv4Router::SetGlobalDataRate(DataRate data_rate){
  
  //Saving value for upcoming requests.
  m_gdata_rate = data_rate;
  
  for (uint32_t index = 0; index < m_ifaces.GetN(); index++)
  {
    Ptr<CsmaChannel> channel = 
    m_ifaces.Get(index)->GetChannel()->GetObject<CsmaChannel>();

    if(channel != nullptr){
      channel->SetAttribute("DataRate", DataRateValue(m_gdata_rate));
    }
  }
}


NetDeviceContainer 
Ipv4Router::GetIfaces(){
  return m_ifaces;
}


Ptr<CsmaNetDevice>      
Ipv4Router::GetIface(std::string iface_name){
  
  Ptr<CsmaNetDevice> iface = 
  Names::Find<CsmaNetDevice>(
    std::to_string(m_node->GetId())+iface_name
  );
  
  if(iface==nullptr){
    NS_LOG_DEBUG(
      "The interface "<<iface_name <<" does not exists, router id: "<< m_node->GetId()
    );
  }

  return iface;
}


DataRate
Ipv4Router::GetGlobalDataRate(){
  return m_gdata_rate;
}


Ptr<CsmaNetDevice>    
Ipv4Router::IfUp(std::string iface_name, Ipv4Address ip, Ipv4Mask mask){
  
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", DataRateValue(m_gdata_rate));

  Ptr<CsmaNetDevice> iface = 
  csma.Install(m_node).Get(0)->GetObject<CsmaNetDevice>();
  
  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4>();
  uint32_t ip_index = ipv4->AddInterface(iface);

  ipv4->AddAddress(
          ip_index,
          Ipv4InterfaceAddress(ip, mask)
        );

  ipv4->SetUp(ip_index);
  
  ipv4->SetForwarding(
    ip_index,
    true
  );

  Names::Add(std::to_string(m_node->GetId())+iface_name, iface);
  
  m_ifaces.Add(iface);

  return iface;
}  

/*
  We pretend to emulate ppoE, therefore, any device wanted to be
  linked to the router must be a CSMA like device. 
*/
Ptr<NetDevice>
Ipv4Router::Link(Ptr<Node> guest, std::string iface_name, Ipv4Address guest_ip, Ipv4Mask guest_mask){

  Ptr<CsmaNetDevice> iface = GetIface(iface_name);
  Ptr<NetDevice> guest_iface;

  if(iface==nullptr){
    NS_LOG_ERROR(
      "The interface "<<iface_name <<" does not exists, router id: "<< m_node->GetId()
    );
  }else{

    CsmaHelper csma;

    guest_iface = csma.Install(
      guest,
      iface->GetChannel()->GetObject<CsmaChannel>()
    ).Get(0);

    Ptr<Ipv4> ipv4 = guest->GetObject<Ipv4>();
    uint32_t ip_index = ipv4->AddInterface(guest_iface);

    ipv4->AddAddress(
          ip_index,
          Ipv4InterfaceAddress(
            guest_ip,
            guest_mask
          )
    );

    ipv4->SetUp(ip_index);
  
    ipv4->SetForwarding(
        ip_index,
        true
    );

  }

  return guest_iface;
}


void
Ipv4Router::EnablePcap(void){

  CsmaHelper csma;

  csma.EnablePcap(
    "router_traces",
    m_ifaces,
    false
  );
    
}


  