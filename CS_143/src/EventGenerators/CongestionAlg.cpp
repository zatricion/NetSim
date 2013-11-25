#include "CongestionAlg.h"
/**
 * Initial call to the congestion algorithm.  It is only called once.  It
 * populates the host's event queue with packets.
 *
 * @param flow the pointer to the flow that called the CongestionAlgorithm.
 */
void CongestionAlg::initialize(Flow* flow) {
    int windowSize = flow->windowSize;
    std::shared_ptr<Host> host = flow->host;
    
    // At the outset, add a number of events equal to the window size.
    // Unless window size is less than number of packest.
    int end = std::min(windowSize, flow->numPackets);
    for (int i = 0; i < end; i++) {
        // The packet corresponding to the sent data.
        // TODO what should the IDs be?  Why do they even have IDs?
        auto p = std::make_shared<Packet>(std::to_string(i), flow->destination,
                                          flow->source, flow->packetSize, false, i, flow->id);

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
}
