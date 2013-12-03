#include "TCPVegas.h"
#include "../EventHandling/TimeoutEvent.h"
#include "../EventHandling/TCPVegasUpdateEvent.h"
#include <algorithm>

/**
 * Called when a packet has not been acknowledged for waitTime.
 * 
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param unacked the unacknowledged packet
 * @param time the time at which the unackedEvent was thrown
 */
void TCPVegas::handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) {

    auto e = std::make_shared<PacketEvent>(flow->host->my_link->getID(), 
        flow->source, time, unacked);
    
    flow->host->sendAndQueueResend(unacked, time, flow->waitTime);
}


/**
 * Called when an ack has been received for a particular flow.
 *
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param pkt the ack packet
 * @param time the time at which the event was received
 */
void TCPVegas::handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) {
    int seqNum = pkt->sequence_num;

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

        flow->host->send(fin, time);
        return;
    }

    // Send packets.
    int windowSize = flow->windowEnd - flow->windowStart;
    flow->windowStart = seqNum;
    flow->windowEnd = std::min(seqNum + windowSize - 1, flow->numPackets - 1);
    sendManyPackets(flow, time);

    flow->logFlowWindowSize(time, flow->windowEnd - flow->windowStart + 1);
}


void TCPVegas::handleVegasUpdate(Flow *flow, float time) {
    FILE_LOG(logDEBUG) << "VEGAS UPDATE EVENT";
    if (flow->phase == DONE || flow->phase == FIN) {
        return;
    }

    int windowSize = flow->windowStart - flow->windowEnd + 1;
    FILE_LOG(logDEBUG) << "BEFORE: windowSize=" << windowSize;
    float testValue = (windowSize / flow->minRTT) - (windowSize / flow->A);
    if (testValue < flow->vegasConstAlpha) { windowSize += 1; }
    else if (testValue > flow->vegasConstBeta) { windowSize -= 1; }
    flow->windowEnd = windowSize + flow->windowStart - 1;
    windowSize = flow->windowStart - flow->windowEnd + 1;

    sendManyPackets(flow, time);

    // schedule another update
    auto update = std::make_shared<TCPVegasUpdateEvent>(flow->source, flow->source,
        time + flow->waitTime, flow->id);
    flow->host->addEventToLocalQueue(update);

    FILE_LOG(logDEBUG) << "AFTER: windowSize=" << windowSize;
    flow->logFlowWindowSize(time, windowSize);
}

std::string TCPVegas::toString() {
    return "TCPVegas";
}
