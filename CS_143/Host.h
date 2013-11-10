#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include "CongestionAlg.h"
#include "Device.h"
#include "Link.h"

#define DATA_PKT_SIZE 0; // TODO: change this number

class Host : public Device
{
public:
    int window_size;
    
    // Lets us know where to interleave new flows
    int num_flows;
    
    // Link this host connects to
    Link& my_link;
    
    // Packets waiting to be sent
    std::queue<Packet> packet_queue;
    
    // Constructor takes a congestion control algorithm and an id string
    Host(CongestionAlg, Link&);
    
    // Add event to eventHeap
    void sendPacket(Packet);
    
    // React to an event
    void giveEvent(std::unique_ptr<Event>);
    
    // Create packets for new flow and interleave them with packet_queue
    void addFlow(std::string dest, float size);
    
};



#endif /* defined(__CS_143__Host__) */
