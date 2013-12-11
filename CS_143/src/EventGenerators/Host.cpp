#include "Host.h"
#include "Flow.h" // Must include here because circular dependency.


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
    std::unordered_map<std::string, std::shared_ptr<Flow> > flows;
    std::unordered_map<std::string, std::pair<std::set<int>, Phase>> recvd;
}


/**
 * Give an event to the Host.  This method determines the type of the event,
 * then hands it to the appropriate helper function.
 *
 * @param e the event to be processed
 */
void Host::giveEvent(std::shared_ptr<Event> e) {
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
    else if (type == "TCP_VEGAS_UPDATE_EVENT") {
        TCPVegasUpdateEvent t = *(std::static_pointer_cast<TCPVegasUpdateEvent>(e));
        auto v = std::static_pointer_cast<VegasFlow>(flows[t.flowID]);
        v->handleVegasUpdate(t.eventTime());
    }
    else {
        assert (false);
    }
}


/**
 * Called when a FlowEvent is received.
 *
 * @param flow_event the FlowEvent received.
 */
void Host::respondTo(FlowEvent flow_event) {
    // Get the flow object, and add it to the map of flows.
    flows[flow_event.flow->id] = flow_event.flow;

    // Start the SYN handshake.
    flows[flow_event.flow->id]->openConnection(flow_event.eventTime());
}


/**
 * Helper function called when an UnackEvent for a SYN is received.
 *
 * @param unack_event the UnackEvent.
 */
void Host::respondToSynUnackEvent(UnackEvent unack_event) {
    std::string flowString = unack_event.packet->flowID;
    assert(flows.count(flowString) > 0);
    flows[flowString]->respondToSynUnackEvent(unack_event.eventTime());
}


/**
 * Helper function called when an UnackEvent for a FIN is received.
 *
 * @param unack_event the UnackEvent.
 */
void Host::respondToFinUnackEvent(UnackEvent unack_event) {
    auto p = unack_event.packet;
    double time = unack_event.eventTime();
    std::string flowString = p->flowID;
    assert(p->fin);
    assert(!p->ack); //  We don't reschedule the sending of an ack.

    // Our FIN might not have been received by the other host.  Check
    // the state of our host.  If the flow is marked as DONE, then we
    // don't need to resend the FIN.
    if (flows.count(p->flowID) && flows[p->flowID]->phase != DONE) {
        sendAndQueueResend(p, time, flows[p->flowID]->wait_time);
    }
    else if (recvd.count(p->flowID) && recvd[p->flowID].second != DONE) {
        // There are no wait times associated with the receiving end of a flow.
        // It isn't really worth storing any, since timeouts for a flow are
        // only used when a FIN is sent.  So, we will just pick a reasonable 
        // value.
        sendAndQueueResend(p, time, .5);
    }
}


/**
 * Called when the host receives an UnackEvent.
 * 
 * @param unack_event the PacketEvent for the packet that might not have been
 * acknowledged
 */
void Host::respondTo(UnackEvent unack_event) {
    auto p = unack_event.packet;
    double time = unack_event.eventTime();
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


/**
 * Helper function called when the host receives a PacketEvent corresponding to
 * a SYN.
 *
 * @param new_event the PacketEvent.
 */
void Host::respondToSynPacketEvent(PacketEvent new_event) {
    std::shared_ptr<Packet> pkt = new_event.packet;
    double time = new_event.eventTime();

    // Make sure this is where the packet should have ended up.
    assert(pkt->final_dest == uuid);
    assert(pkt->syn); 

    if (pkt->ack) {
        // This host is the sending end of the flow, and it received a SYNACK.
        flows[pkt->flowID]->respondToSynPacketEvent(pkt, time);
    }

    if (!pkt->ack && recvd.count(pkt->flowID) == 0) {
        // This host is the sending end of the flow.

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


/**
 * Helper function called when the host receives a PacketEvent corresponding to
 * a FIN.
 *
 * @param new_event the PacketEvent.
 */
void Host::respondToFinPacketEvent(PacketEvent new_event) {
    std::shared_ptr<Packet> pkt = new_event.packet;
    double time = new_event.eventTime();
    
    // Make sure this is where the packet should have ended up.
    assert(pkt->final_dest == uuid);
    assert(pkt->fin);

    if (pkt->ack) {
        // Received a FINACK.  Set the connection to DONE.
        if (flows.count(pkt->flowID)) {
            // This host is the sending end of the flow.
            flows[pkt->flowID]->phase = DONE;
        }
        else {
            // This host is the receiving end of the flow.
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
                
            // There are no wait times associated with the receiving end of a 
            // flow.

            // It isn't really worth storing any, since timeouts for a flow are
            // only used when a FIN is sent.  So, we will just pick a reasonable 
            // value.
            sendAndQueueResend(fin, time, .5);
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
    std::shared_ptr<Packet> pkt = new_event.packet;
    double time = new_event.eventTime();
    
    // make sure this is where the packet should have ended up
    assert(pkt->final_dest == uuid);

    if (pkt->bf_tbl_bit) {
        // Do nothing
        return;
    }
    
    else if (pkt->syn) {
        respondToSynPacketEvent(new_event);
    }

    else if (pkt->fin) {
        respondToFinPacketEvent(new_event);
    }

    else {
        // Not a syn or a fin or a bf packet.  It's a data packet, or an ack.
        if (pkt->ack && flows.count(pkt->flowID)) {
            // This host is the sending end of the flow.
    	    flows[pkt->flowID]->handleAck(pkt, time);

            // Note that if the host is the receiving end, do nothing.
            // This is because when we receive the ACK from the SYNACK,
            // we do nothing.
        }
        if (!pkt->ack) {
            // Log the flow rate
            flow_received[pkt->flowID] += pkt->size;
            if (time - last_flow_log[pkt->flowID] > 0.1) {
                logFlowRate(time, pkt->flowID);
                last_flow_log[pkt->flowID] = time;
                last_flow_received[pkt->flowID] = flow_received[pkt->flowID];
            }
            
            // We received a packet.  Send an acknowledgment.
            recvd[pkt->flowID].first.insert(pkt->sequence_num);
            
            // Find out what our sequence number should be.

            // It should be the first packet missing from our list of received
            // packets.
            int ackNum = 0;
            for (auto it = recvd[pkt->flowID].first.begin(); 
                 it != recvd[pkt->flowID].first.end(); it ++) {
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
 * Sends a packet by creating a PacketEvent for the packet and placing the
 * PacketEvent on the event_heap.
 *
 * @param pkt the Packet to be sent.
 * @param time the time at which the Packet should be sent.
 */
void Host::send(std::shared_ptr<Packet> pkt, double time) {
    assert(time > 0);
    auto pEV = std::make_shared<PacketEvent>(my_link->getID(), uuid, time, pkt);
    addEventToLocalQueue(pEV);
}


/**
 * Queues a PacketEvent in the event_heap, as well as an UnackEvent for the 
 * PacketEvent.
 *
 * @param pkt the packet for the PacketEvent
 * @param time the time at which event is to be sent.
 * @param delay the UnackEvent will trigger at time + delay
 */
void Host::sendAndQueueResend(std::shared_ptr<Packet> pkt, double time,
                              float delay) {
    assert(pkt->timestamp <= time);
    assert(time > 0); 
    assert(delay > 0);

    send(pkt, time);
    auto uEV = std::make_shared<UnackEvent>(pkt, uuid, uuid, time + delay);
    addEventToLocalQueue(uEV);
}

/**
* Logs the flow rate by looking at the number of bits received for each flow
* in the time between logs for that flow.
*
* @param time the time at which the data is logged.
* @param flowID the name of the flow
*/
void Host::logFlowRate(double time, std::string flowID) {
    double bits_received = flow_received[flowID] - last_flow_received[flowID];
    double flow_rate = bits_received / (time - last_flow_log[flowID]);
    sim_plotter.logFlowRate(flowID, std::make_tuple(time, flow_rate));
}



