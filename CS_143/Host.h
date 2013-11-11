#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include <cassert>
#include "CongestionAlg.h"
#include "Device.h"
#include "Link.h"
#include "Packet.h"
#include "MultiQueue.h"
#include <stdio.h>

using namespace std;

class Host : public Device
{
public:
    int window_size;
    
    // Congestion Control algorithm host runs
    CongestionAlg congestion_alg;
    
    // Link this host connects to
    string my_link;
    
    // Constructor takes a congestion control algorithm and an id string
    Host(CongestionAlg, Link&);
    
    // Packets waiting to be sent
    MultiQueue<Packet> packet_queue;
    
    // Add event to eventHeap
    void sendPacket(Packet);
    
    // React to an event
    void giveEvent(unique_ptr<FlowEvent>);
    void giveEvent(unique_ptr<PacketEvent>);

    // Create packets for new flow and interleave them with packet_queue
    void addFlow(string, float, float);
    
    // Create Packet_IDS
    int packet_id;
    
};




#endif /* defined(__CS_143__Host__) */
