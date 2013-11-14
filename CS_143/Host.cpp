#include "Host.h"
#include "Flow.h"
#include <math.h> // ceil
#include <string>
  
Host::Host(std::shared_ptr<Link> host_link, std::string host_id) : my_link(host_link)
{
    my_link = host_link;
    uuid = host_id;
    // Try parens TODO
    // Made a default constructor for Flow
    std::unordered_map<std::string, Flow > flows;
}

void Host::giveEvent(std::shared_ptr<Event> e)
{
    std::string type = e->getType();
    if (type == "PACKET_EVENT")
    {
            respondTo(*(std::static_pointer_cast<PacketEvent>(e)));
    }
    else if (type == "FLOW_EVENT")
    {
            respondTo(*(std::static_pointer_cast<FlowEvent>(e)));
    }
    else if (type == "UNACK_EVENT")
    {
            respondTo(*(std::static_pointer_cast<UnackEvent>(e)));
    }
    else
    {
        printf("ERROR");
    }
}

/*
 * Called when a flow_event is received.
 */
void Host::respondTo(FlowEvent flow_event)
{
    // Get a flow object, and add it to the map of flows.
    flows[flow_event.floww->id] = *(flow_event.floww);
    flows[flow_event.floww->id].initialize();
    // TODO
    std::shared_ptr<Event> ee = eventHeap.top();
    (std::static_pointer_cast<PacketEvent>(ee))->packet.printPacket();
}

/**
 * Called when there is a potentially unacknowledged packet.
 */
void Host::respondTo(UnackEvent unack_event)
{
	flows[unack_event.packet.flowID].handleUnackEvent(unack_event.packet, unack_event.eventTime());
}

/**
 * Called when host receives any packet.
 */
void Host::respondTo(PacketEvent new_event)
{
    Packet pkt = new_event.packet;
    
    if (pkt.ack)
    {
    	flows[pkt.flowID].handleAck(pkt, new_event.eventTime());
    }
    // We received a packet.  Send an acknowledgment.
    else {
    	// TODO what is pkt.id??
        
        // Create empty table to pass by reference
        std::map<std::string, std::vector<std::string> > table;
        
    	Packet ret(pkt.uuid, pkt.source, pkt.final_dest, pkt.size, true, pkt.sequence_num);
        //@MaxHorton TODO eventually, we will have to make sure that these
        //events are not all occurring simulatneously (not violating the link
        // rate by sending several events to the link in the span of 1ms).
    	float ts = new_event.eventTime();
    	auto pEv = std::make_shared<PacketEvent>(my_link->getID(), getID(), ts, ret);
        addEventToLocalQueue(pEv);
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
