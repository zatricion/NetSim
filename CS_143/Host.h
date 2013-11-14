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
    std::shared_ptr<Link> my_link;
    
    // Associates flow IDs with Flow objects.
    std::unordered_map<std::string, Flow > flows;

    // Constructor
    Host(std::shared_ptr<Link> host_link, std::string host_id);
   
    // React to an event
    virtual void giveEvent(std::shared_ptr<Event>) override;
    
    // giveEvent helper functions
    void respondTo(PacketEvent);
    void respondTo(FlowEvent);
    void respondTo(UnackEvent);
    
    std::string toString();
};

#endif /* defined(__CS_143__Host__) */
