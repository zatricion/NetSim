#include "Host.h"
#include "Flow.h"
#include <math.h> // ceil
#include <string>
#include <cassert>
#include "../Tools/Log.h"
  
/**
 * Constructor.
 * 
 * @param host_link the link connecting the host to a router
 * @param host_id the unique id of the host
 */
Host::Host(std::shared_ptr<Link> host_link, std::string host_id) : 
    my_link(host_link) {

    my_link = host_link;
    uuid = host_id;
    std::unordered_map<std::string, Flow > flows;
}


/**
 * Receives an event.  Determines its type, and passes to a helper function.
 *
 * @param e the event to be processed
 */
void Host::giveEvent(std::shared_ptr<Event> e) {
    FILE_LOG(logDEBUG4) << "Host with id=" << uuid << " received an event.";
    std::string type = e->getType();
    if (type == "PACKET_EVENT") {
        respondTo(*(std::static_pointer_cast<PacketEvent>(e)));
    }
    else if (type == "FLOW_EVENT") {
        respondTo(*(std::static_pointer_cast<FlowEvent>(e)));
    }
    else if (type == "UNACK_EVENT") {
        respondTo(*(std::static_pointer_cast<UnackEvent>(e)));
    }
    else {
        assert (false);
    }
}


/**
 * Called when a flow_event is received.
 *
 * @param flow_event the flow event received
 */
void Host::respondTo(FlowEvent flow_event) {
    FILE_LOG(logDEBUG) << "Host with id=" << uuid << " received a FlowEvent.";
    // Get a flow object, and add it to the map of flows.
    flows[flow_event.floww->id] = flow_event.floww;
    flows[flow_event.floww->id]->initialize();
}


/**
 * Called when there is a potentially unacknowledged packet.
 * 
 * @param unack_event the PacketEvent for the packet that might not have been
 * acknowledged
 */
void Host::respondTo(UnackEvent unack_event) {
    FILE_LOG(logDEBUG) << "Host with id=" << uuid << " received an UnackEvent.";
    // Find the appropriate flow, and have it handle the event.
    flows[unack_event.packet->flowID]->handleUnackEvent(unack_event.packet, 
        unack_event.eventTime());
}


/**
 * Called when host receives a packet.
 *
 * @param new_event the packet event received.
 */
void Host::respondTo(PacketEvent new_event) {
    FILE_LOG(logDEBUG) << "Host with id=" << uuid << " received a PacketEvent.";
    std::shared_ptr<Packet> pkt = new_event.packet;
    FILE_LOG(logDEBUG) << "Packet contents:" << pkt->toString();
    
    if (pkt->ack)
    {
    	flows[pkt->flowID]->handleAck(pkt, new_event.eventTime());
    }
    // We received a packet.  Send an acknowledgment.
    else {
    	auto ret = std::make_shared<Packet>(pkt->uuid, pkt->source, 
            pkt->final_dest, pkt->size, true, pkt->sequence_num, pkt->flowID);
        //@MaxHorton TODO eventually, we will have to make sure that these
        //events are not all occurring simulatneously (not violating the link
        // rate by sending several events to the link in the span of 1ms).
    	float ts = new_event.eventTime();
    	auto pEv = std::make_shared<PacketEvent>(my_link->getID(), getID(), ts, ret);
        addEventToLocalQueue(pEv);
    }
}


/**
 * Get a string representing the host.
 *
 * @return the string representing the host
 */
std::string Host::toString() {
    // ID, all flows, all waiting outgoing messages, etc.
    // TODO this implementation sucks
    std::string ret(uuid);
    for (auto& flowStringPair : flows) {
        ret += flowStringPair.second->toString() + "\n";
    }
    return ret;
}
