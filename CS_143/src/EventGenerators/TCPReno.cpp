#include "TCPReno.h"
/**
 * Called when a packet has not been acknowledged for waitTime.
 * 
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param unacked the unacknowledged packet
 * @param time the time at which the unackedEvent was thrown
 */
void TCPReno::handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) {
    // the new PacketEvent to be queued
    auto e = std::make_shared<PacketEvent>(flow->host->my_link->getID(), 
        flow->source, time, unacked);
    
    flow->host->addEventToLocalQueue(e);
    FILE_LOG(logDEBUG) << "Handled UnackEvent.  Flow:" << flow->toString();
    // TODO update window size, etc.
}


/**
 * Called when an ack has been received for a particular flow.
 *
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param pkt the ack packet
 * @param time the time at which the event was received
 */
void TCPReno::handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) {
    std::shared_ptr<Host> host = flow->host;

    if (flow->numAcked == flow->numPackets || 
        flow->unAckedPackets.count(pkt->sequence_num) == 0) {
        // First case: we are already done (received all ACKs).  Do nothing.
        // Send case: this packet was already acknowledged.  Do nothing.
        FILE_LOG(logDEBUG) << "Handled Ack with no-op.";
        return;
    }

    // Case: we have no new things to send, but haven't received all the
    // other acks yet.
    // TODO combine this with above statement.

    FILE_LOG(logDEBUG) << "flow->numAcked=" << flow->numAcked;
    FILE_LOG(logDEBUG) << "flow->unAckedPackets.size()=" << flow->unAckedPackets.size();
    if (flow->numAcked + (int) flow->unAckedPackets.size() == flow->numPackets) {
        if (flow->unAckedPackets.count(pkt->sequence_num) == 0) {
            // Packet was already acked.  Do nothing.
        }
        else {
            flow->numAcked = flow->numAcked + 1;
            flow->unAckedPackets.erase(pkt->sequence_num);
        }
        return;
    }
            
    flow->numAcked = flow->numAcked + 1;
    flow->unAckedPackets.erase(pkt->sequence_num);

    // Update the window size.  TODO

    // Find the next packet to send.  It should be the packet with the
    // lowest number, that has NOT been sent yet.
    int nextToSend = *(flow->unAckedPackets.rbegin()) + 1;
    FILE_LOG(logDEBUG) << "Will send packet with sequence_num=" << nextToSend;

    // ensure that it has a valid number.  Valid numbers are 
    // {0, ..., flow->numPackets - 1}
    assert (nextToSend < flow->numPackets);

    // Create the packet, send it
    auto p = std::make_shared<Packet>(std::to_string(nextToSend), 
        flow->destination, flow->source, flow->packetSize, false,
        nextToSend, flow->id);
    
    auto pe = std::make_shared<PacketEvent>(host->my_link->getID(),
        flow->source, time, p);
    host->addEventToLocalQueue(pe);

    flow->unAckedPackets.insert(nextToSend);

    // Add another UnackEvent to the queue.
    auto ue = std::make_shared<UnackEvent>(p, host->my_link->getID(), 
        flow->source, time + flow->waitTime);
    host->addEventToLocalQueue(ue);
    FILE_LOG(logDEBUG) << "Handled new ack.  Flow:" << flow->toString();
}
//TODO other considerations:
// sometimes we will add more than one event, upon receiving an ack (if our
// window size changes.  I don't think we will ever add an event without
// receiving an ack, right...?
