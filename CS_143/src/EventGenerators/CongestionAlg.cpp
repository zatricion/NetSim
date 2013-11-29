#include "CongestionAlg.h"
/**
 * Initial call to the congestion algorithm.  It is only called once.  It
 * populates the host's event queue with packets.
 *
 * @param flow the pointer to the flow that called the CongestionAlgorithm.
 */
void CongestionAlg::initialize(Flow* flow) {
    //int windowSize = flow->windowSize;
    //std::shared_ptr<Host> host = flow->host;
    
    // At the outset, add a number of events equal to the window size.
    // Unless window size is less than number of packets.
    FILE_LOG(logDEBUG) << "Initializing flow:" << flow->toString();
    // TODO
    sendManyPackets(flow, 500);
    
/*
    int end = std::min(windowSize, flow->numPackets);
    for (int i = 0; i < end; i++) {
        // The packet corresponding to the sent data.
        // TODO what should the IDs be?  Why do they even have IDs?
        auto p = std::make_shared<Packet>(std::to_string(i), flow->destination,
            flow->source, flow->packetSize, false, i, flow->id, false, false);

        // TODO the timestamps will all be the same, unless we add 
        // some value.  This should be i times the link delay, but we need
        // more changes to get that to work anyway.

        // Make and add the event.
        auto e = std::make_shared<PacketEvent>(host->my_link->getID(),
            flow->source, flow->timestamp + i, p);
        host->addEventToLocalQueue(e);

        // These packets should be added to the unAckedPackets
        flow->unAckedPackets.insert(i);

        // Add an UnackEvent to fire when we are tired of waiting for an ack.
        auto ue = std::make_shared<UnackEvent>(p, host->getID(), flow->source,
            flow->timestamp + i * host->my_link->getPropDelay() + flow->waitTime);
        host->addEventToLocalQueue(ue);
    }
    FILE_LOG(logDEBUG) << "Initialized data stream.  Flow:" << flow->toString();
*/
}

/**
 * Sends packets until the unAckedPackets list has a length equal to the window
 * size.
 */
void CongestionAlg::sendManyPackets(Flow* flow, float time) {
    // TODO 
    // Send every packet within the transmission window that has never been
    // sent before.
    for (int i = flow->windowStart; i <= flow->windowEnd; i++) {
        if (flow->unSentPackets.count(i) == 1) {
            // He has not been sent before.  Let's send him, and queue a resend.
        flow->unSentPackets.erase(i);
        auto p = std::make_shared<Packet>(std::to_string(i), flow->destination, flow->source, flow->packetSize, false, i, flow->id, false, false);
        // TODO what time should they be at??  We're fucked.
        flow->host->sendAndQueueResend(p, time, flow->waitTime);
    }
}



    /*

    FILE_LOG(logDEBUG) << "unAckedPackets.size() " << flow->unAckedPackets.size();
    FILE_LOG(logDEBUG) << "unSentPackets.size() " << flow->unSentPackets.size();
    while ((int) flow->unAckedPackets.size() < flow->windowSize && flow->unSentPackets.size() > 0) {
        // Get the lowest number of the unsent packets.
        int lowestUnsent = *(flow->unSentPackets.begin());
        flow->unSentPackets.erase(lowestUnsent);

        auto p = std::make_shared<Packet>(std::to_string(lowestUnsent), flow->destination,
            flow->source, flow->packetSize, false, lowestUnsent, flow->id, false, false);

        // Make and add the event.
        auto e = std::make_shared<PacketEvent>(flow->host->my_link->getID(),
            flow->source, flow->timestamp, p);
        flow->host->addEventToLocalQueue(e);

        // These packets should be added to the unAckedPackets
    FILE_LOG(logDEBUG) << "INLOOP B4: unAckedPackets.size() " << flow->unAckedPackets.size();
        flow->unAckedPackets.insert(lowestUnsent);
    FILE_LOG(logDEBUG) << "INLOOP AFT: unAckedPackets.size() " << flow->unAckedPackets.size();

        // Add an UnackEvent to fire when we are tired of waiting for an ack.
        auto ue = std::make_shared<UnackEvent>(p, flow->host->getID(), flow->source,
            flow->timestamp + flow->waitTime);
        flow->host->addEventToLocalQueue(ue);
        FILE_LOG(logDEBUG) << "Sent a packet: " << p->toString();
    }
    FILE_LOG(logDEBUG) << "Done sending packet.  AFTERWARDS:";
    FILE_LOG(logDEBUG) << "unAckedPackets.size() " << flow->unAckedPackets.size();
    FILE_LOG(logDEBUG) << "unSentPackets.size() " << flow->unSentPackets.size();
    */
}
