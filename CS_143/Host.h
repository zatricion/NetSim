#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include <cassert>
#include "CongestionAlg.h"
#include "Link.h"
#include "Packet.h"
#include "MultiQueue.h"
#include "FlowEvent.h"
#include "UnackEvent.h"
#include <unordered_set>
#include <unordered_map>

class Host : public EventGenerator
{
public:
    // Link this host connects to
    Link& my_link;
    
    // Associates flow IDs with Flow objects.
    std::unordered_map<std::string, Flow> flow;

    // Constructor
    Host(Link& host_link);
   
    // TODO this should be in EventGenerator.cpp.  Why isn't it?
    // Add event to local priority queue.
    void addEventToLocalQueue(Event e);
    
    // React to an event
    void giveEvent(std::unique_ptr<FlowEvent>);
    void giveEvent(std::unique_ptr<PacketEvent>);
    void giveEvent(std::unique_ptr<UnackEvent>);
};

#endif /* defined(__CS_143__Host__) */
