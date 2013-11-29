#include "TCPReno.h"
#include "../EventHandling/TCPRenoUpdateEvent.h"
#include <algorithm>
/**
 * Called when a packet has not been acknowledged for waitTime.
 * 
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param unacked the unacknowledged packet
 * @param time the time at which the unackedEvent was thrown
 */
void TCPReno::handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) {
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
    int seqNum = pkt->sequence_num;
    if (seqNum == flow->numPackets - 1) {
        // TODO
        FILE_LOG(logDEBUG) << "DONEEEEE.";
        return;
    }

    if (flow->renoPhase == SLOWSTART) {
        int windowSize = flow->windowEnd - flow->windowStart + 1;
        // increase window size by 1
        windowSize += 1;
        flow->windowStart = seqNum;
        flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
        sendManyPackets(flow, time);
        if (windowSize > flow->ssthresh) {
            flow->renoPhase = CONGESTIONAVOIDANCE;
        }
        return;
    }

    else if (flow->renoPhase == CONGESTIONAVOIDANCE) {
    if (seqNum == flow->windowStart) {
        flow->multiplicity += 1;
        if (flow->multiplicity >= 3) { // TODO make sure you're in correct state
            flow->multiplicity = 0;
            flow->renoPhase = FASTRECOVERY;
            int oldWindowEnd = flow->windowEnd;
            int windowSize = flow->windowEnd - flow->windowStart + 1;
            windowSize /= 2;
            flow->windowSize /= 2;
            flow->windowEnd = flow->windowStart + windowSize;
            // All the packets that were just cut out of the transmission
            // window need to be added to the list of unsent packets, so they
            // aren't forgotten.
            for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
                flow->unSentPackets.insert(i);
            }
            // Now, we want to do a fast retransmit.
            for (int i = flow->windowStart; i <= flow->windowEnd; i++) {
                auto p = std::make_shared<Packet>("FRETRANS", flow->destination, flow->source, 0, false, i, flow->id, false, false); //TODO not 0
                auto pEV = std::make_shared<PacketEvent>(flow->host->my_link->getID(), flow->host->getID(), time, p);
                flow->host->addEventToLocalQueue(pEV);
                // Don't add resend.
            }

            // TODO increase windowsize by 1 every rtt.
            auto timeout = std::make_shared<TCPRenoUpdateEvent>(flow->host->getID(), flow->host->getID(), time, flow->windowStart);
            flow->host->addEventToLocalQueue(timeout);
        }
        else {
            
        int windowSize = flow->windowEnd - flow->windowStart + 1;
        // increase window size by 1
        flow->windowStart = seqNum;
        flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
        sendManyPackets(flow, time);
        if (windowSize > flow->ssthresh) {
            flow->renoPhase = CONGESTIONAVOIDANCE;
        }
        }
        return;
    }}

    else if (flow->renoPhase == FASTRECOVERY) {
        flow->renoPhase = CONGESTIONAVOIDANCE;
        return;
    }
            


    FILE_LOG(logDEBUG) << "Handled new ack, sequence_num=" << pkt->sequence_num << ".  Flow:" << flow->toString();


    // Take the diff of the sets.  Set the flow->unAckedPackets to be
    //std::set<int> diff;
    //std::set<int> unAcked = flow->unAckedPackets;
    //std::set<int> acked = pkt->ackSet;
    //std::set_difference(unAcked.begin(), unAcked.end(), acked.begin(), acked.end(), std::inserter(diff, diff.end()));
    // Set the unAckedPackets to be the difference.
    //flow->unAckedPackets = diff;
    // Send more packets if applicable.
    //sendManyPackets(flow);



    /*
    // Update window size:
    if (flow->renoPhase == SLOWSTART) { // TODO make sure it was a new ack.
        flow->windowSize = flow->windowSize + 1;
        if (flow->windowSize > flow->ssthresh) {
            flow->renoPhase = CONGESTIONAVOIDANCE;
            // TODO throw an update event.
        }
    }

    
    else if (flow->renoPhase == CONGESTIONAVOIDANCE) {
        int seqnum = pkt->sequence_num;
        if (flow->multiplicityOfAcksReceived.count(seqnum) == 0) {
            flow->multiplicityOfAcksReceived[seqnum] = 0;
        }
        flow->multiplicityOfAcksReceived[seqnum] += 1;
        if (flow->multiplicityOfAcksReceived[seqnum] >= 4) {
            flow->multiplicityOfAcksReceived.clear();
            flow->windowSize /= 2;
            flow->ssthresh = flow->windowSize;
            flow->renoPhase = FASTRECOVERY;
            // TODO this whole algorithm sort of expects GO BACK N, but we're
            // doing selective acks...  Make sure that things like this are
            // working properly.
            // For now, we plow onward.
        }
    */
            

    /*
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

    sendManyPackets(flow);

    */
    /*
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
        nextToSend, flow->id, false, false);
    
    auto pe = std::make_shared<PacketEvent>(host->my_link->getID(),
        flow->source, time, p);
    host->addEventToLocalQueue(pe);

    flow->unAckedPackets.insert(nextToSend);

    // Add another UnackEvent to the queue.
    auto ue = std::make_shared<UnackEvent>(p, host->my_link->getID(), 
        flow->source, time + flow->waitTime);
    host->addEventToLocalQueue(ue);
    FILE_LOG(logDEBUG) << "Handled new ack.  Flow:" << flow->toString();
    */
}
//TODO other considerations:
// sometimes we will add more than one event, upon receiving an ack (if our
// window size changes.  I don't think we will ever add an event without
// receiving an ack, right...?
