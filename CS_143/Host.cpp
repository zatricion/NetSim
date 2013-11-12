#include "Host.h"
#include "Flow.h"
#include <math.h> // ceil
  
Host::Host(Link& host_link) : my_link(host_link)
{
    my_link = host_link;
    std::unordered_map<std::string, Flow> flows;
}

/**
 * Add an event to local priority queue.
 */
// TODO this belongs in EventGenerator
void Host::addEventToLocalQueue(Event e) {
    eventHeap.push(e);
}


/*
 * Called when a flow_event is received.
 */
void Host::giveEvent(std::unique_ptr<FlowEvent> flow_event)
{
    // Create a Flow object, and add it to the map of flows.
    flows.insert(flow_event->flow.id, flow_event->flow);
}

/**
 * Called when there is a potentially unacknowledged packet.
 */
void Host::giveEvent(std::unique_ptr<UnackEvent> unack_event)
{
	flows.find(unack_event->flow.id).handleUnackEvent(unack_event->packet, unack_event->eventTime());
        // TODO delete the unack_event
}

/**
 * Called when host receives any packet.
 */
void Host::giveEvent(std::unique_ptr<PacketEvent> new_event)
{
    Packet pkt = new_event->packet;
    
    if (pkt.ack)
    {
    	flows.find(new_event->flow.id).handleAck(pkt, new_event->eventTime());
    }
    // We received a packet.  Send an acknowledgment.
    else {
    	// TODO what is pkt.id??
    	Packet ret(pkt.uuid, pkt.source, pkt.final_dest, pkt.size, true, false, false, NULL, pkt.sequence_num);
        //@MaxHorton TODO eventually, we will have to make sure that these
        //events are not all occurring simulatneously (not violating the link
        // rate by sending several events to the link in the span of 1ms).
    	float ts = new_event->eventTime();
    	PacketEvent pEv(my_link.getID(), getID(), ts, ret);
        addEventToLocalQueue(pEv);
    }
}
