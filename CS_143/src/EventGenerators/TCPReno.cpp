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
    
    // TODO this is the correct behavior, right?
    flow->host->sendAndQueueResend(unacked, time, flow->waitTime);
    FILE_LOG(logDEBUG) << "Handled UnackEvent.  Flow:" << flow->toString();
}


/**
 * Called when an ack has been received for a particular flow.
 *
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param pkt the ack packet
 * @param time the time at which the event was received
 */
void TCPReno::handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) {
    int seqNum = pkt->sequence_num;

    if (seqNum == flow->numPackets - 1) {
        // TODO we need to use FINs, but for now, reaching this point seems
        // like a noble enough goal.
        FILE_LOG(logDEBUG) << "DONE WITH DATA FLOW.";
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
        }
        return;
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

            int oldWindowEnd = flow->windowEnd;

            // Halve the window size.
            int windowSize = flow->windowEnd - flow->windowStart + 1;
            windowSize /= 2;
            flow->windowEnd = flow->windowStart + windowSize;

            // All the packets that were just cut out of the transmission
            // window need to be added to the list of unsent packets, so they
            // aren't forgotten.
            for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
                flow->unSentPackets.insert(i);
            }

            // Now, we want to do a fast retransmit.
            for (int i = flow->windowStart; i <= flow->windowEnd; i++) {
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
            auto timeout = std::make_shared<TCPRenoUpdateEvent>(
                flow->host->getID(), flow->host->getID(), time, 
                flow->windowStart);
            flow->host->addEventToLocalQueue(timeout);
        }

        else {
            // We have not received triple acks.  We will send more packets,
            // after adjusting the window bounds.
        
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
    } // end CONGESTIONAVOIDANCE block;

    else if (flow->renoPhase == FASTRECOVERY) {
        flow->renoPhase = CONGESTIONAVOIDANCE;
        return;
    }
            


    FILE_LOG(logDEBUG) << "Handled new ack, sequence_num=" << pkt->sequence_num << ".  Flow:" << flow->toString();

}
//TODO other considerations:
// sometimes we will add more than one event, upon receiving an ack (if our
// window size changes.  I don't think we will ever add an event without
// receiving an ack, right...?
