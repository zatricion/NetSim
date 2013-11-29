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
    std::unordered_map<std::string, std::pair<std::set<int>, Phase>> recvd;
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
        // This should be where we add code for reno update events.
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

    // Instead of initializing, we want to start the handshake.
    auto syn = std::make_shared<Packet>("SYN", flow_event.floww->destination,
        flow_event.floww->source, flow_event.floww->packetSize, false, -1, 
        flow_event.floww->id, true, false);
    sendAndQueueResend(syn, flow_event.eventTime(), 500);
    

    //flows[flow_event.floww->id]->initialize(); TODO
}


/**
 * Called when there is a potentially unacknowledged packet.
 * 
 * @param unack_event the PacketEvent for the packet that might not have been
 * acknowledged
 */
void Host::respondTo(UnackEvent unack_event) {
    auto p = unack_event.packet;
    float time = unack_event.eventTime();
    std::string flowString = p->flowID;

    if (p->syn) {
        FILE_LOG(logDEBUG) << "Host with id=" << uuid << "received SYN unackEvent.";
        if (p->ack) {
            // This shouldn't happen.  The SYN.ACKs shouldn't throw
            // UnackEvents.
            assert(false);
        }
        else {
            // We know the SYN doesn't need to be resent if we got a SYN.ACK,
            // which occurs if our proper flow is no longer in "SYN" mode.
            assert(flows.count(flowString) > 0);
            if (flows[flowString]->phase == SYN) {
                FILE_LOG(logDEBUG) << "Resending SYN";
                // We need to resend.
                sendAndQueueResend(p, time, 500);
            }
        }
    }

    else {
        // It's a data packet.
        FILE_LOG(logDEBUG) << "Host with id=" << uuid << " received an UnackEvent.";
        // Find the appropriate flow, and have it handle the event.
        flows[p->flowID]->handleUnackEvent(p, time);
    }
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
    float time = new_event.eventTime();
    

    if (pkt->syn) {
        if (pkt->ack) {
            // received syn.ack.  First, we send an ack.  Then, we start            // sending data, and set the mode of the flow to DATA.
            if (flows[pkt->flowID]->phase != DATA) {
                FILE_LOG(logDEBUG) << "Received a first SYNACK.  On host:" << toString();
                // This is the first SYN.ACK received.
                flows[pkt->flowID]->phase = DATA;
                auto ack = std::make_shared<Packet>("ACK", pkt->source, uuid,
                    pkt->size, true, -1, pkt->flowID, false, false);
                auto pEV = std::make_shared<PacketEvent>(my_link->getID(), 
                    uuid, time, ack);
                addEventToLocalQueue(pEV);

                FILE_LOG(logDEBUG) << "pre init" << flows[pkt->flowID]->unSentPackets.size();
                flows[pkt->flowID]->initialize();
                FILE_LOG(logDEBUG) << "post init" << flows[pkt->flowID]->unSentPackets.size();
            }
        }
        else {
            // received a syn, non-ack.
            // Send a syn.ack.  Then, create an entry in recvd.  Do these things
            // only if we don't already have an entry in recvd (i.e. only if we
            // have never gotten a non-ack syn before.
            if (recvd.count(pkt->flowID) == 0) {
                recvd[pkt->flowID] = std::pair<std::set<int>, Phase>(std::set<int>(), DATA);
                auto synack = std::make_shared<Packet>("SYNACK", pkt->source,
                    uuid, pkt->size, true, -1, pkt->flowID, true, false);
                auto pEV = std::make_shared<PacketEvent>(my_link->getID(), 
                    uuid, time, synack);
                addEventToLocalQueue(pEV);
            }
        }
    }

    else {
    // Data packet.


    if (pkt->ack) {
        FILE_LOG(logDEBUG) << "Received an ack.";
        // There are two cases.
        if (flows.count(pkt->flowID)) {
            // The receiver of the ack is the sending end of the flow.
    	    flows[pkt->flowID]->handleAck(pkt, new_event.eventTime());
        }
        else {
            // The receiver of the ack is the receiving end of the flow.
            // Do nothing.
            assert(recvd.count(pkt->flowID));
            FILE_LOG(logDEBUG) << "Handled ack with noop.";
            // TODO should never happen for data packets.
            // TODO we don't need to do anything.  We just received an ACK
            // from the source telling us it got our SYNACK.  We don't even have
            // anything to dequeue.
        }
    }
    // We received a packet.  Send an acknowledgment.
    else {
        // TODO choose the sequence number more carefully.
        recvd[pkt->flowID].first.insert(pkt->sequence_num);
    	auto ret = std::make_shared<Packet>(pkt->uuid, pkt->source, 
            pkt->final_dest, pkt->size, true, pkt->sequence_num + 1, pkt->flowID, false, false);
        //@MaxHorton TODO eventually, we will have to make sure that these
        //events are not all occurring simulatneously (not violating the link
        // rate by sending several events to the link in the span of 1ms).
        ret->ackSet = recvd[pkt->flowID].first;
    	float ts = new_event.eventTime();
    	auto pEv = std::make_shared<PacketEvent>(my_link->getID(), getID(), ts, ret);
        addEventToLocalQueue(pEv);
    }
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

/**
 *
 */
void Host::sendAndQueueResend(std::shared_ptr<Packet> pkt, float time, float delay) {
    auto pEV = std::make_shared<PacketEvent>(my_link->getID(), uuid, time, pkt);
    addEventToLocalQueue(pEV);
    auto uEV = std::make_shared<UnackEvent>(pkt, uuid, uuid, time + delay);
    addEventToLocalQueue(uEV);
}
