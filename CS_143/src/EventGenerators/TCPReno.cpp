#include "TCPReno.h"
#include "../EventHandling/TimeoutEvent.h"
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
    FILE_LOG(logDEBUG) << "Handling UnackEvent for packet " << 
        unacked->toString() << ".  Flow:" << flow->toString();
    auto e = std::make_shared<PacketEvent>(flow->host->my_link->getID(), 
        flow->source, time, unacked);
    
    // TODO this is the correct behavior, right?
    flow->host->sendAndQueueResend(unacked, time, flow->waitTime);
    FILE_LOG(logDEBUG) << "Handled UnackEvent.  Flow " << flow->toString();
}


/**
 * Called when an ack has been received for a particular flow.
 *
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param pkt the ack packet
 * @param time the time at which the event was received
 */
void TCPReno::handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) {
    FILE_LOG(logDEBUG) << "Handling an ack.  Packet " << pkt->toString();
    int seqNum = pkt->sequence_num;

    if (seqNum == flow->numPackets) {
        // We received an ack that requests a nonexistent packet.  I.e. we sent
        // the 100th packet (with seqNum 99), and this ack says "100", but there
        // is no 100th packet to send.  So we're done.

        // We're done sending packets.  Move the window outside of the bounds
        // of the packets.
        flow->windowStart = seqNum;
        flow->windowEnd = seqNum;

        FILE_LOG(logDEBUG) << "DONE WITH DATA FLOW."; // We might not actually be done.
        // We appear to still be getting more events after this.  TODO
        flow->phase = FIN;
        // We need to send a FIN to the other host.
        auto fin = std::make_shared<Packet>("FIN", flow->destination,
            flow->source, pkt->size, false, -1, flow->id, false, true);
        auto finEvent = std::make_shared<PacketEvent>(
            flow->host->my_link->getID(), flow->host->getID(), time, fin);
        flow->host->addEventToLocalQueue(finEvent);
            
        return;
    }

    // Handle each case separately, depending on which phase of Reno we're in.

    if (flow->renoPhase == SLOWSTART) {
        int windowSize = flow->windowEnd - flow->windowStart + 1;
        // increase window size by 1, then readjust window and send packets.
        windowSize += 1;
        flow->windowStart = seqNum;
        flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
        sendManyPackets(flow, time);
        if (windowSize > flow->ssthresh) {
            flow->renoPhase = CONGESTIONAVOIDANCE;
            flow->cavCount += 1;
            // Create an update event, in which the window size
            // increments once every RTT.
            auto up = std::make_shared<TCPRenoUpdateEvent>(flow->source, 
                flow->source, time + flow->waitTime, flow->cavCount, flow->id);
            flow->host->addEventToLocalQueue(up);
        }
    }

    else if (flow->renoPhase == CONGESTIONAVOIDANCE) {
        if (seqNum == flow->windowStart) {
            flow->multiplicity += 1;
        }

        if (flow->multiplicity >= 3) {
            // We have received triple acks.  Move into fast recovery, do a
            // fast resend.  If we don't receive an ack, we will go back to
            // slow start.
            flow->multiplicity = 0;
            flow->renoPhase = FASTRECOVERY;
            flow->fastWindowEnd = flow->windowEnd;
            flow->frCount += 1;

            int oldWindowEnd = flow->windowEnd;

            // Halve the window size.
            int windowSize = flow->windowEnd - flow->windowStart + 1;
            windowSize /= 2;
            flow->windowEnd = flow->windowStart + windowSize - 1;

            // All the packets that were just cut out of the transmission
            // window need to be added to the list of unsent packets, so they
            // aren't forgotten.
            for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
                flow->unSentPackets.insert(i);
            }

            // Now, we want to do a fast retransmit.  Not clear to me if we
            // resend all packets, or just the first.  It might just be first.
            // Hence, the funny "loop"
            //for (int i = flow->windowStart; i <= flow->windowEnd; i++) {
            for (int i = flow->windowStart; i == flow->windowStart; i++) {
                // TODO not 0.
                auto p = std::make_shared<Packet>("FRETRANS", flow->destination,
                    flow->source, 0, false, i, flow->id, false, false);
                auto pEV = std::make_shared<PacketEvent>(
                    flow->host->my_link->getID(), flow->host->getID(), time, p);
                flow->host->addEventToLocalQueue(pEV);
            }

            // TODO increase windowsize by 1 every rtt.

            // Now, we add a timeout event.  If we don't get any acks from our
            // fast retransmit, the timeout event will fire and we go back to
            // slow start.
            auto timeout = std::make_shared<TimeoutEvent>(
                flow->host->getID(), flow->host->getID(), time, 
                flow->frCount, flow->id);
            flow->host->addEventToLocalQueue(timeout);
        }

        else {
            // We have not received triple acks.  We will send more packets,
            // after adjusting the window bounds.
        
            int windowSize = flow->windowEnd - flow->windowStart + 1;
            flow->windowStart = seqNum;
            flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
            sendManyPackets(flow, time);
        }
    } // end CONGESTIONAVOIDANCE block;

    else if (flow->renoPhase == FASTRECOVERY) {
        //if (seqNum <= flow->windowEnd && seqNum > flow->windowStart) {
        if (seqNum > flow->fastWindowEnd) {
            // We have received acks for everything in the window.  Recovery
            // was successful.
            flow->renoPhase = CONGESTIONAVOIDANCE;
            flow->cavCount += 1;

            // Set up updates for every RTT
            auto up = std::make_shared<TCPRenoUpdateEvent>(flow->source, 
                flow->source, time + flow->waitTime, flow->cavCount, flow->id);
            flow->host->addEventToLocalQueue(up);

            // Now, do what we normally would in the CONGESTIONAVOIDANCE phase.
            handleAck(flow, pkt, time);
        }
        // Otherwise, we haven't received acks for everything in the window.
        // continue to wait for more acks or the TimeoutEvent, whichever comes
        // first.  (We don't want to congest the network by sending more packets
        // at this time).  TODO are we supposed to send packets?  It's unclear.
    }
}

void TCPReno::handleRenoUpdate(Flow *flow, int cavCount, float time) {
    FILE_LOG(logDEBUG) << "handling RenoUpdate.";
    if (cavCount == flow->cavCount &&
        flow->renoPhase == CONGESTIONAVOIDANCE &&
        flow->phase == DATA) {
        
        // Increase the window size by 1
        flow->windowEnd = std::min(flow->windowEnd + 1, flow->numPackets - 1);
        sendManyPackets(flow, time);
        
        // Schedule another update
        auto up = std::make_shared<TCPRenoUpdateEvent>(flow->source, 
            flow->source, time + flow->waitTime, flow->cavCount, flow->id);
        flow->host->addEventToLocalQueue(up);
    }
}

void TCPReno::handleTimeout(Flow *flow, int frCount, float time) {
    FILE_LOG(logDEBUG) << "handling TimeoutEvent.";
    if (frCount == flow->frCount &&
        flow->renoPhase == FASTRECOVERY) {
        // Reduce window size to 1
        int oldWindowEnd = flow->windowEnd;
        int windowSize = 1;
        flow->windowEnd = flow->windowStart + windowSize - 1;
        // All the packets that were just cut out of the transmission
        // window need to be added to the list of unsent packets, so they
        // aren't forgotten.
        for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
            flow->unSentPackets.insert(i);
        }

        flow->renoPhase = SLOWSTART;
    }
}
