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
    // TODO this change may have made a huge difference.
    std::unordered_map<std::string, std::shared_ptr<Flow> > flows;
    std::unordered_map<std::string, std::pair<std::set<int>, Phase>> recvd;
}


/**
 * Receives an event.  Determines its type, and passes to a helper function.
 *
 * @param e the event to be processed
 */
void Host::giveEvent(std::shared_ptr<Event> e) {
    std::string type = e->getType();
    if (type == "PACKET_EVENT") {
        FILE_LOG(logDEBUG) << "Got a packet event.";
        respondTo(*(std::static_pointer_cast<PacketEvent>(e)));
    }
    else if (type == "FLOW_EVENT") {
        respondTo(*(std::static_pointer_cast<FlowEvent>(e)));
    }
    else if (type == "UNACK_EVENT") {
        respondTo(*(std::static_pointer_cast<UnackEvent>(e)));
    }
    else if (type == "TCP_RENO_UPDATE_EVENT") {
        assert(false);
        //TCPRenoUpdateEvent t = *(std::static_pointer_cast<TCPRenoUpdateEvent>(e));
        //flows[t.flowID]->handleRenoUpdate(t.congAvCount, t.eventTime());
    }
    else if (type == "TCP_VEGAS_UPDATE_EVENT") {
        TCPVegasUpdateEvent t = *(std::static_pointer_cast<TCPVegasUpdateEvent>(e));
        auto v = std::static_pointer_cast<VegasFlow>(flows[t.flowID]);
        v->handleVegasUpdate(t.eventTime());
    }
    else if (type == "TIMEOUT_EVENT") {
        assert(false);
        //TimeoutEvent t = *(std::static_pointer_cast<TimeoutEvent>(e));
        //flows[t.flowID]->handleTimeout(t.fastRecoveryCount, t.eventTime());
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
    FILE_LOG(logDEBUG1) << "Host with id=" << uuid << " received a FlowEvent.";
    // Get a flow object, and add it to the map of flows.
    flows[flow_event.floww->id] = flow_event.floww;

    // Here, we want to start the SYN handshake.
    flows[flow_event.floww->id]->openConnection(flow_event.eventTime());
}


void Host::respondToSynUnackEvent(UnackEvent unack_event) {
    std::string flowString = unack_event.packet->flowID;
    assert(flows.count(flowString) > 0);
    flows[flowString]->respondToSynUnackEvent(unack_event.eventTime());
}


void Host::respondToFinUnackEvent(UnackEvent unack_event) {
    auto p = unack_event.packet;
    float time = unack_event.eventTime();
    std::string flowString = p->flowID;
    assert(p->fin);
    assert(!p->ack); //  We don't reschedule the sending of an ack.

    // Our FIN might not have been received by the other host.  Check
    // the state of our host.  If the flow is marked as DONE, then we
    // don't need to resend the FIN.
    if (flows.count(p->flowID) && flows[p->flowID]->phase != DONE) {
        sendAndQueueResend(p, time, flows[p->flowID]->waitTime);
    }
    else if (recvd.count(p->flowID) && recvd[p->flowID].second != DONE) {
        sendAndQueueResend(p, time, 2); // TODO there are no wait times
                                        // associated with the receiving end.
                                        // so I picked a number.
    }
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
        respondToSynUnackEvent(unack_event);
    }
    else if (p->fin) {
        respondToFinUnackEvent(unack_event);
    }
    else {
        // Find the appropriate flow, and have it handle the event.
        flows[p->flowID]->handleUnackEvent(p, time);
    }
}


void Host::respondToSynPacketEvent(PacketEvent new_event) {
    std::shared_ptr<Packet> pkt = new_event.packet;
    float time = new_event.eventTime();
    
    // make sure this is where the packet should have ended up
    assert(pkt->final_dest == uuid);
    assert(pkt->syn); 

    if (pkt->ack) {
        flows[pkt->flowID]->respondToSynPacketEvent(pkt, time);
    }

    if (!pkt->ack && recvd.count(pkt->flowID) == 0) {
        // received a syn, non-ack.
        // Send a syn.ack.  Then, create an entry in recvd.  Do these things
        // only if we don't already have an entry in recvd (i.e. only if we
        // have never gotten a non-ack syn before.
        recvd[pkt->flowID] = 
            std::pair<std::set<int>, Phase>(std::set<int>(), DATA);
        auto synack = std::make_shared<Packet>("SYNACK",
                                               pkt->source, // final destination
                                               uuid, // source
                                               SYN_SIZE,
                                               true, // ack packet?
                                               -1, // sequence number
                                               pkt->flowID,
                                               true, // syn packet?
                                               false, // fin packet?
                                               pkt->timestamp);
            
        send(synack, time);
    }
}


void Host::respondToFinPacketEvent(PacketEvent new_event) {
    std::shared_ptr<Packet> pkt = new_event.packet;
    
    // make sure this is where the packet should have ended up
    assert(pkt->final_dest == uuid);
    assert(pkt->fin);

    float time = new_event.eventTime();

    if (pkt->ack) {
        // Received a FIN.ACK.  Set the connection to DONE.
        if (flows.count(pkt->flowID)) {
            // This host is the sending end of the flow.
            flows[pkt->flowID]->phase = DONE;
        }
        else {
            assert(recvd.count(pkt->flowID));
            recvd[pkt->flowID].second = DONE;
        }
        return;
    }

    // else: received a fin, non-ack
    if (recvd.count(pkt->flowID)) {
        // This host is the receiving end of the flow.
        // We received a FIN from the sending end.  This is how
        // the receiving end knows that the flow is over.
        
        if (recvd[pkt->flowID].second == DATA) {
            recvd[pkt->flowID].second = FIN;
            auto fin = std::make_shared<Packet>("FIN",
                                                pkt->source, // final destination
                                                uuid, // source
                                                FIN_SIZE,
                                                false, // ack packet?
                                                -1, // sequence number
                                                 pkt->flowID,
                                                 false, // syn packet?
                                                 true, // fin packet?
                                                 time);
                
            // TODO we need to store wait times for each recvd object
            // in addition to the other stuff we already have.
            // or we can just do this lolz
            sendAndQueueResend(fin, time, 2);
        }

    }

    // Regardless of whether we were already in the FIN state or
    // not, we want to resend a FINACK.
    auto finack = std::make_shared<Packet>("FINACK",
                                           pkt->source, // final destination
                                           uuid, // source
                                           FIN_SIZE,
                                           true, // ack packet?
                                           -1, // sequence number
                                           pkt->flowID,
                                           false, // syn packet?
                                           true, // fin packet?
                                           pkt->timestamp);
    send(finack, time);
}


/**
 * Called when host receives a packet.
 *
 * @param new_event the packet event received.
 */
void Host::respondTo(PacketEvent new_event) {
    FILE_LOG(logDEBUG) << "Packet event received on host=" << uuid;
    std::shared_ptr<Packet> pkt = new_event.packet;
    float time = new_event.eventTime();
    
    // make sure this is where the packet should have ended up
    assert(pkt->final_dest == uuid);

    if (pkt->bf_tbl_bit) {
        // Do nothing
    }
    
    else if (pkt->syn) {
        FILE_LOG(logDEBUG) << "It was a syn";
        respondToSynPacketEvent(new_event);
    }

    else if (pkt->fin) {
        respondToFinPacketEvent(new_event);
        FILE_LOG(logDEBUG) << "It was a fin";
    }

    else {
        FILE_LOG(logDEBUG) << "It was a data packet.";
        FILE_LOG(logDEBUG) << pkt->toString() << ", gotten on host" << uuid;
        // Not a syn or a fin or a bf packet.  It's a data packet.
        if (pkt->ack && flows.count(pkt->flowID)) {
            FILE_LOG(logDEBUG) << "Flow is handling hack.";
            // The receiver of the ack is the sending end of the flow.
    	    flows[pkt->flowID]->handleAck(pkt, time);
            // Note that if the host is the receiving end, do nothing.
            // This is because when we receive the ACK from the SYNACK,
            // we do nothing.
        }
        if (!pkt->ack) {
            FILE_LOG(logDEBUG) << "Receiver (host) is handling hack.";
            // We received a packet.  Send an acknowledgment.
            recvd[pkt->flowID].first.insert(pkt->sequence_num);
            
            // Find out what our sequence number should be.
            int ackNum = 0;
            for (auto it = recvd[pkt->flowID].first.begin(); it != recvd[pkt->flowID].first.end(); it ++) {
                if (*it > ackNum) { break; }
                ackNum += 1;
            }

    	    auto ret = std::make_shared<Packet>(pkt->uuid,
                                                pkt->source, // final destination
                                                uuid, // source
                                                ACK_SIZE,
                                                true, // ack packet?
                                                ackNum,
                                                pkt->flowID,
                                                false, // syn packet?
                                                false, // fin packet?
                                                pkt->timestamp);
            
            send(ret, time);
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
    // TODO wtf is this nonsense.
    for (auto& flowStringPair : flows) {
        ret += flowStringPair.second->toString() + "\n";
    }
    return ret;
}

void Host::send(std::shared_ptr<Packet> pkt, float time) {
    FILE_LOG(logDEBUG) << "send from uuid=" << uuid;
    auto pEV = std::make_shared<PacketEvent>(my_link->getID(), uuid, time, pkt);
    addEventToLocalQueue(pEV);
}


/**
 * Queues a packetEvent, and an unack event for the packetEvent.
 *
 * @param pkt the packet for the PacketEvent
 * @param time the time at which event is to be sent.
 * @param delay the packet will be sent at time + delay
 */
void Host::sendAndQueueResend(std::shared_ptr<Packet> pkt, float time, float delay) {
    FILE_LOG(logDEBUG) << "sendAndQueueResend";
    //auto pEV = std::make_shared<PacketEvent>(my_link->getID(), uuid, time, pkt);
    //addEventToLocalQueue(pEV);
    send(pkt, time);

    auto uEV = std::make_shared<UnackEvent>(pkt, uuid, uuid, time + delay);
    addEventToLocalQueue(uEV);
}
