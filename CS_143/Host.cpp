#include "Host.h"
#include "Flow.h"
#include <math.h> // ceil
#include <string>
  
Host::Host(Link& host_link, std::string host_id) : my_link(host_link)
{
    my_link = host_link;
    uuid = host_id;
    // Try parens TODO
    // Made a default constructor for Flow
    std::unordered_map<std::string, Flow > flows;
}

void Host::giveEvent(std::shared_ptr<Event>) {};

/*
 * Called when a flow_event is received.
 */
void Host::giveEvent(std::shared_ptr<FlowEvent> flow_event)
{
    // Get a flow object, and add it to the map of flows.
    flows[flow_event->floww->id] = *(flow_event->floww);
}

/**
 * Called when there is a potentially unacknowledged packet.
 */
void Host::giveEvent(std::shared_ptr<UnackEvent> unack_event)
{
        Packet p = unack_event->packet;
	flows[p.flowID].handleUnackEvent(unack_event->packet, unack_event->eventTime());
}

/**
 * Called when host receives any packet.
 */
void Host::giveEvent(std::shared_ptr<PacketEvent> new_event)
{
    Packet pkt = new_event->packet;
    
    if (pkt.ack)
    {
    	flows[pkt.flowID].handleAck(pkt, new_event->eventTime());
    }
    // We received a packet.  Send an acknowledgment.
    else {
    	// TODO what is pkt.id??
        
        // Create empty table to pass by reference
        std::map<std::string, std::vector<std::string> > table;
        
    	Packet ret(pkt.uuid, pkt.source, pkt.final_dest, pkt.size, true, false, false, table, pkt.sequence_num);
        //@MaxHorton TODO eventually, we will have to make sure that these
        //events are not all occurring simulatneously (not violating the link
        // rate by sending several events to the link in the span of 1ms).
    	float ts = new_event->eventTime();
    	PacketEvent pEv(my_link.getID(), getID(), ts, ret);
        addEventToLocalQueue(std::make_shared<PacketEvent>(pEv));
    }
}

std::string Host::toString() {
    // ID, all flows, all waiting outgoing messages, etc.
    std::string ret(uuid);
    for (auto& flowStringPair : flows) {
        ret += flowStringPair.second.toString() + "\n";
    }
    return ret;
}
