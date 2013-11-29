#include "CongestionAlg.h"
/**
 * Initial call to the congestion algorithm.  It is only called once.  It
 * populates the host's event queue with packets.
 *
 * @param flow the pointer to the flow that called the CongestionAlgorithm.
 */
void CongestionAlg::initialize(Flow* flow, float time) {
    FILE_LOG(logDEBUG) << "Initializing flow:" << flow->toString();
    sendManyPackets(flow, time);
}


/**
 * Sends all packets between flow->windowStart and flow->windowEnd that have
 * never been sent.  Also queues resend events in case they don't make it.
 *
 * @param flow the pointer to the flow that called the CongestionAlgorithm.
 * @param time the time associated with the events to be queued.
 */
void CongestionAlg::sendManyPackets(Flow* flow, float time) {
    for (int i = flow->windowStart; i <= flow->windowEnd; i++) {
        if (flow->unSentPackets.count(i) == 1) {
            // He has not been sent before.  Let's send him, and queue a resend.
            flow->unSentPackets.erase(i);
            auto p = std::make_shared<Packet>(std::to_string(i), 
                flow->destination, flow->source, flow->packetSize,
                false, i, flow->id, false, false);
        flow->host->sendAndQueueResend(p, time, flow->waitTime);
        }
    }
}
