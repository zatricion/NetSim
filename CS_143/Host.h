#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include "CongestionAlg.h"
#include "Device.h"
#include "Link.h"
#include "Packet.h"

class Host : public Device
{
public:
    int window_size;
    
    // Lets us know where to interleave new flows
    int num_flows;
    
    // Congestion Control algorithm host runs
    CongestionAlg congestion_alg;
    
    // Link this host connects to
    Link& my_link;
    
    // Constructor takes a congestion control algorithm and an id string
    Host(CongestionAlg, Link&);
    
    // Packets waiting to be sent
    std::vector<Packet> packet_queue;
    
    // Add event to eventHeap
    void sendPacket(Packet);
    
    // React to an event
    void giveEvent(std::unique_ptr<FlowEvent>);
    void giveEvent(std::unique_ptr<PacketEvent>);

    // Create packets for new flow and interleave them with packet_queue
    void addFlow(std::string, float, float);
    
    // Create Packet_IDS
    int packet_id;
    
};



#endif /* defined(__CS_143__Host__) */
