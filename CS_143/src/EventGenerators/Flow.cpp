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
Flow::Flow(std::string idval, std::string dest, int data_size,
           std::shared_ptr<Host> h, int winSize, double ts) {
    // Set fields.
    timestamp = ts;
    host = h;
    id = idval;
    source = h->getID();
    destination = dest;
    numPackets = ceil(1.0 * data_size / DATA_PKT_SIZE);

    // Default wait time is set to .5 seconds.  The waitTime will be updated
    // upon reception of acks.
    waitTime = .5;
    phase = SYN;
    unSentPackets = std::set<int>();
    
    // Populate unSentPackets with every packet.
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
    for (int i = windowStart; i <= windowEnd; i++) {
        if (unSentPackets.count(i) == 1) {
            // Packet i has not been sent before.  Send it, and queue a resend.
            unSentPackets.erase(i);

            // We want the packets to have slightly different timestamps, so
            // that their order is retained.  So, we add a small number to each
            // packet time.  This will be smaller than any realistic network
            // delay.
            double cpuTime = i / 10000000000.0;
            auto pkt = std::make_shared<Packet>(std::to_string(i), destination,
                source, DATA_PKT_SIZE, false, i, id, false, false,
                time + cpuTime);
            host->sendAndQueueResend(pkt, time + cpuTime, waitTime);
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


/**
 * Log the RTT for a received packet.
 *
 * @param time the time at which the packet is received.
 * @param RTT the round trip time to be logged.
 */
void Flow::logFlowRTT(double time, double RTT) {
    FILE_LOG(logDEBUG) << "logFlowRTT: " << time << ", " << RTT;
    sim_plotter.logFlowRTT(id,
        std::make_tuple(time, RTT));
}


/**
 * Log the window size of the flow.
 *
 * @param time the time at which the window size is logged
 * @param windowSize the window size
 */
void Flow::logFlowWindowSize(double time, int windowSize) {
    FILE_LOG(logDEBUG) << "logFlowWindowSize: " << time << ", " << (double) windowSize;
    sim_plotter.logFlowWindowSize(id,
        std::make_tuple(time, (double) windowSize));
}


/**
 * Begins the data transfer for the flow by sending a SYN.
 *
 * @param time the time at which the data transfer begins.
 */
void Flow::openConnection(double time) {
    auto syn = std::make_shared<Packet>("SYN",
                                        destination,
                                        source,
                                        SYN_SIZE,
                                        false, // ack packet?
                                        -1, // sequence number
                                        id,
                                        true, // syn packet?
                                        false, // bf packet?
                                        time);

    sendAndQueueResend(syn, time, waitTime);
}


/**
 * Respond to an UnackEvent from a SYN packet.
 *
 * @param time the time at which the UnackEvent is received.
 */
void Flow::respondToSynUnackEvent(double time) {
    // Check if synack has been received.
    if (phase == SYN) {
        FILE_LOG(logDEBUG) << "SYNACK not received.  Resending SYN.";
        openConnection(time);
    }
}
