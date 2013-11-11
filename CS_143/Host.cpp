#include "Host.h"
#include <math.h> // ceil
  
static const int INITIAL_WINDOW_SIZE = 0; // TODO: change this number
static const int DATA_PKT_SIZE = 1; // TODO: change this number

Host::Host(CongestionAlg congestion_algorithm , Link& host_link, float rt) : my_link(host_link)
{
    window_size = INITIAL_WINDOW_SIZE;
    congestion_alg = congestion_algorithm;
    my_link = host_link;
    packet_id = 0;
    unack_packets = 0;
    RTO = rt;
}

/**
 * Add an event to local priority queue.
 */
void Host::addEventToLocalQueue(Event e) {
    eventHeap.push(e);
}
/*
 * Add an event to local priority queue.
 */
/*
void Host::sendPacket(Packet new_pack, int time_now)
{
    // Insert packet to the eventHeap
    PacketEvent packet_event = PacketEvent(new_pack, new_pack.final_dest, new_pack.source, time_now);
    eventHeap.push(packet_event);
    

    // Insert to the eventHeap an event which is triggered once a packet has timed out
    UnackEvent unack_event = UnackEvent(new_pack, this->getId(), this->getId(), time_now + RTO);
    eventHeap.push(unack_event);
    
    unacknowledged_packets.insert(new_pack.uuid);
    unack_packets++;
}
*/

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
	flows.find(unack_event->flow.id).handleUnackEvent();
}

/**
 * Called when host receives any packet.
 */
void Host::giveEvent(std::unique_ptr<PacketEvent> new_event)
{
    Packet pkt = new_event->packet;
    
    if (pkt.ack)
    {
    	flows.find(new_event->flow.id).handleAck(pkt);
    }
    // We received a packet.  Send an acknowledgment.
    else {
    	// TODO what is pkt.id??
    	Packet ret(pkt.id, pkt.src, pkt.dest, pkt.s, true, false, pkt.seq_num);
        //@MaxHorton TODO eventually, we will have to make sure that these
        //events are not all occurring simulatneously (not violating the link
        // rate by sending several events to the link in the span of 1ms).
    	float ts = new_event->eventTime();
    	PacketEvent pEv(ret, my_link.getId(), this->getId(), ts);
    }
}
