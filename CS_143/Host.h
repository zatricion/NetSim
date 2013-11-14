#ifndef __CS_143__Host__
#define __CS_143__Host__

class Flow;
//class FlowEvent;

#include <iostream>
#include <cassert>
#include "Link.h"
#include "Packet.h"
#include "FlowEvent.h"
#include "Flow.h"
#include "UnackEvent.h"
#include <unordered_set>
#include <unordered_map>
#include <string>

class Host : public EventGenerator
{
public:    
    // Link this host connects to
    Link& my_link;
    
    // Associates flow IDs with Flow objects.
    std::unordered_map<std::string, Flow > flows;

    // Constructor
    Host(Link& host_link, std::string host_id);
   
    // React to an event
    void giveEvent(Event) {};
    void giveEvent(FlowEvent);
    void giveEvent(PacketEvent);
    void giveEvent(UnackEvent);
    std::string toString();
};

#endif /* defined(__CS_143__Host__) */
