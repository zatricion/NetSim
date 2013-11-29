#ifndef __CS_143__Host__
#define __CS_143__Host__

class Flow;
//class FlowEvent;

#include <iostream>
#include <cassert>
#include "Link.h"
#include "../EventHandling/Packet.h"
#include "../EventHandling/FlowEvent.h"
#include "Flow.h"
#include "../EventHandling/UnackEvent.h"
#include "../EventHandling/TCPRenoUpdateEvent.h"
#include "../EventHandling/TimeoutEvent.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "../Tools/Log.h"

class Host : public EventGenerator
{
public:    
    // Link this host connects to
    std::shared_ptr<Link> my_link;
    
    // Associates flow IDs with Flow objects.
    std::unordered_map<std::string, std::shared_ptr<Flow> > flows;

    // Keeps track of what information has been received by which flows.
    std::unordered_map<std::string, std::pair<std::set<int>, Phase>> recvd;

    // Constructor
    Host(std::shared_ptr<Link> host_link, std::string host_id);
   
    // React to an event
    virtual void giveEvent(std::shared_ptr<Event>) override;
    
    // giveEvent helper functions
    void respondTo(PacketEvent);
    void respondTo(FlowEvent);
    void respondTo(UnackEvent);
    void sendAndQueueResend(std::shared_ptr<Packet> pkt, float time, float delay);
    
    std::string toString();
};

#endif /* defined(__CS_143__Host__) */
