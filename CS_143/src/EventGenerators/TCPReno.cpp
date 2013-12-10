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

    //if (unacked->sequence_num != flow->windowStart) { return; }
    if (unacked->timestamp < flow->ignoreTime) { FILE_LOG(logDEBUG) << "UNACK Ignored."; return; }
    flow->ignoreTime = time;

    FILE_LOG(logDEBUG) << "HANDLINGUNACK: " << flow->toString();
    FILE_LOG(logDEBUG) << "NUM=" << unacked->sequence_num;

    unacked->timestamp = time;
    // If in slowstart or cong av, go to slow start windowsize->1
    // change ssthresh to half previous win size.
    // 
    if (flow->renoPhase == FASTRECOVERY) {
        assert(false);
        return;
    }
    FILE_LOG(logDEBUG) << "TIMEOUT";
    flow->renoPhase = SLOWSTART;
    flow->ssthresh = (flow->windowEnd - flow->windowStart + 1) / 2;
    // Set the window size to 1.
    //int oldWindowEnd = flow->windowEnd;
    flow->windowEnd = flow->windowStart;
    // TODO
    //for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
        //flow->unSentPackets.insert(i);
    //}

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
    FILE_LOG(logDEBUG1) << "Handling an ack.  Packet " << pkt->toString();
    int seqNum = pkt->sequence_num;
    if (seqNum < flow->windowStart) { return; }

    if (seqNum == flow->numPackets) {
        // We received an ack that requests a nonexistent packet.  I.e. we sent
        // the 100th packet (with seqNum 99), and this ack says "100", but there
        // is no 100th packet to send.  So we're done.

        // We're done sending packets.  Move the window outside of the bounds
        // of the packets.
        flow->windowStart = seqNum;
        flow->windowEnd = seqNum;

        FILE_LOG(logDEBUG1) << "DONE WITH DATA FLOW.";
        flow->phase = FIN;
        // We need to send a FIN to the other host.
        auto fin = std::make_shared<Packet>("FIN", flow->destination,
            flow->source, FIN_SIZE, false, -1, flow->id, false, true, time);
        auto finEvent = std::make_shared<PacketEvent>(
            flow->host->my_link->getID(), flow->host->getID(), time, fin);
        flow->host->addEventToLocalQueue(finEvent);
            
    }

    // Handle each case separately, depending on which phase of Reno we're in.

    if (flow->renoPhase == SLOWSTART) {
        FILE_LOG(logDEBUG) << "In SLOWSTART";
        int windowSize = flow->windowEnd - flow->windowStart + 1;
        // increase window size by 1, then readjust window and send packets.
        windowSize += 1;
        flow->windowStart = seqNum;
        flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
        sendManyPackets(flow, time);
        if (windowSize > flow->ssthresh) {
            FILE_LOG(logDEBUG) << "Entering CONGESTIONAVOIDANCE";
            flow->renoPhase = CONGESTIONAVOIDANCE;
            //flow->cavCount += 1;
            // Create an update event, in which the window size
            // increments once every RTT.
            //auto up = std::make_shared<TCPRenoUpdateEvent>(flow->source, 
                //flow->source, time + flow->waitTime, flow->cavCount, flow->id);
            //flow->host->addEventToLocalQueue(up);
            // TODO instead you can do + 1 / windowSize.
            // To reconcile rounding: keep floating point, use integer.
            // can also have window size in bytes; division would work.
        }
    }

    else if (flow->renoPhase == CONGESTIONAVOIDANCE) {
        if (seqNum == flow->windowStart) {
            flow->multiplicity += 1;
        }
        else {
            assert(seqNum > flow->windowStart);
            flow->multiplicity = 0;
        }

        if (flow->multiplicity > 3) {
            // We have received triple acks.  Do a fast resend, then timeout.
            flow->multiplicity = 0;

            FILE_LOG(logDEBUG) << "TRIPLICATE ACKS FOR " << pkt->toString();
            auto retrans = std::make_shared<Packet>(
                std::to_string(pkt->sequence_num),
                pkt->source,
                pkt->final_dest,
                DATA_PKT_SIZE,
                false,
                pkt->sequence_num,
                pkt->flowID,
                false, false, time);
            handleUnackEvent(flow, retrans, time);
        }

        else {
            // We have not received triple acks.  We will send more packets,
            // after adjusting the window bounds.
            int windowSize = flow->windowEnd - flow->windowStart + 1;

            float winOverFlow = flow->winOverFlow;

            assert(flow->renoPhase == CONGESTIONAVOIDANCE);
            FILE_LOG(logDEBUG) << "windowSize=" << windowSize;
            winOverFlow += 1.0 / (windowSize);
            windowSize += (int) winOverFlow;
            winOverFlow -= (int) winOverFlow;
            FILE_LOG(logDEBUG) << "windowSize=" << windowSize << ", winOverFlow=" << winOverFlow;

            flow->winOverFlow = winOverFlow;
            flow->windowStart = seqNum;
            flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
            sendManyPackets(flow, time);
        }
    } // end CONGESTIONAVOIDANCE block;

    // log the window size.
    flow->logFlowWindowSize(time, flow->windowEnd - flow->windowStart + 1);
}

void TCPReno::handleRenoUpdate(Flow *flow, int cavCount, float time) {
    assert(false);
    FILE_LOG(logDEBUG1) << "handling RenoUpdate.";
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
    flow->logFlowWindowSize(time, flow->windowEnd - flow->windowStart + 1);
}

void TCPReno::handleTimeout(Flow *flow, int frCount, float time) {
    assert(false);
    FILE_LOG(logDEBUG1) << "handling TimeoutEvent.";
    if (frCount == flow->frCount &&
        flow->renoPhase == FASTRECOVERY) {

        // Reduce window size to 1
        int oldWindowEnd = flow->windowEnd;
        int windowSize = 1;
        flow->windowEnd = flow->windowStart + windowSize - 1;
        flow->ssthresh = (flow->windowEnd - flow->windowStart + 1) / 2;
        // All the packets that were just cut out of the transmission
        // window need to be added to the list of unsent packets, so they
        // aren't forgotten.
        for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
            flow->unSentPackets.insert(i);
        }

        flow->renoPhase = SLOWSTART;
    }
    flow->logFlowWindowSize(time, flow->windowEnd - flow->windowStart + 1);
}

std::string TCPReno::toString() {
    return "TCPReno";
}
