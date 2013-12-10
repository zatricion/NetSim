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
#include "VegasFlow.h"
#include "../EventHandling/UnackEvent.h"
#include "../EventHandling/TCPRenoUpdateEvent.h"
#include "../EventHandling/TCPVegasUpdateEvent.h"
#include "../EventHandling/TimeoutEvent.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "../Tools/Log.h"

static const int ACK_PKT_SIZE = 64 * 8;

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
    void respondTo(PacketEvent new_event);
    void respondTo(FlowEvent flow_event);
    void respondTo(UnackEvent unack_event);
    void sendAndQueueResend(std::shared_ptr<Packet> pkt, double time, float delay);
    void send(std::shared_ptr<Packet> pkt, double time);

    void respondToSynUnackEvent(UnackEvent unack_event);
    void respondToFinUnackEvent(UnackEvent unack_event);
    void respondToSynPacketEvent(PacketEvent new_event);
    void respondToFinPacketEvent(PacketEvent new_event);
    
    // Log flow rate
    void logFlowRate(double time, std::string flowID);

private:
    // Flow received in bits for each flow
    std::unordered_map<std::string, double> flow_received;
    
    // Flow received in bits at the last time the flow was logged
    std::unordered_map<std::string, double> last_flow_received;
    
    // Last time the flow was logged for each flow
    std::unordered_map<std::string, double> last_flow_log;
};

#endif /* defined(__CS_143__Host__) */
