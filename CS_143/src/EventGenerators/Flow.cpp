#include "Flow.h"
#include "Host.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"

/**
 * Constructor for the flow.
 *
 * @param idval the id of the flow.  IDs must be unique
 * @param dest the destination of the data flow
 * @param data_size the size of the flow's data packets
 * @param h the host that is the flow's source
 * @param winSize the initial size of the flow's window (usually 1)
 * @param ts the timestamp at which the flow begins
 */
Flow::Flow(std::string idval, std::string dest,
           int data_size,
           std::shared_ptr<Host> h, int winSize, double ts) {
    timestamp = ts;
    host = h;
    id = idval;
    source = h->getID();
    destination = dest;
    numPackets = ceil(1.0 * data_size / DATA_PKT_SIZE);
    waitTime = .5;
    phase = SYN;
    unSentPackets = std::set<int>();
    for (int i = 0; i < numPackets; i++) {
        unSentPackets.insert(i);
    }
    windowStart = 0;
    windowEnd = winSize - 1;
    A = waitTime;
    D = waitTime;
    b = .1;
}


/**
 * Start the data flow.
 *
 * @param time the time at which the data flow starts.
 */
void Flow::initialize(double time) {
    sendManyPackets(time);
}


/**
 * Send as many packets as possible.  Every packet that has never been marked
 * as being sent (i.e. every packet that is in unsentPackets) that also
 * satisfies windowStart <= sequence_num <= windowEnd will be sent.  The ones
 * that have already been sent (the ones not in unsentPackets) will not be
 * resent.
 *
 * @param time the time at which the packets are sent.
 */
void Flow::sendManyPackets(double time) {
//    std::cout << "windowStart: " << windowStart << " windowEnd: " << windowEnd << " diff: " << windowEnd - windowStart << " time: " << time << std::endl;
    
    for (int i = windowStart; i <= windowEnd; i++) {
        if (unSentPackets.count(i) == 1) {
            // He has not been sent before.  Let's send him, and queue a resend.
            unSentPackets.erase(i);
            auto pkt = std::make_shared<Packet>(std::to_string(i),
                destination, source, DATA_PKT_SIZE,
                false, i, id, false, false, time + i / 10000000000.0);
                // TODO the above looks kind of ghetto, but it's important to
                // offset the packets a bit, to keep them from reshuffling in
                // the buffers.
        FILE_LOG(logDEBUG) << "Called from sendManyPackets";
        FILE_LOG(logDEBUG) << "waitTime=" << waitTime;
        host->sendAndQueueResend(pkt, time + i / 10000000000.0, waitTime);
        }
    }
    
}


/**
 * Helper function for sending a packet from the flow's source to its
 * destination.
 *
 * @param pkt the packet to be sent
 * @param time the time at which it is sent
 */
void Flow::send(std::shared_ptr<Packet> pkt, double time) {
    host->send(pkt, time);
}


/**
 * Sends a packet from the flow's source to its destination.  Also queues
 * an UnackEvent in case an ack isn't received.
 *
 * @param pkt the packet to be sent
 * @param time the time at which the packet it sent
 * @param delay the delay after 'time' at which we consider the packet to have
 * timed out
 */
void Flow::sendAndQueueResend(std::shared_ptr<Packet> pkt, double time, float delay) {
    host->sendAndQueueResend(pkt, time, delay);
}
