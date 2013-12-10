#include "Flow.h"
#include "VegasFlow.h"
#include "Host.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"


/**
 * Constructor for instance of Vegas algorithm.
 *
 * @param idval the unique id of the flow.
 * @param dest the destination of the flow.
 * @param data_size the size of the data transfer.
 * @param h the host on which the flow is generated.
 * @param winSize the initial window size (usually 1).
 * @param ts the timestamp at which the flow is generated.
 */
VegasFlow::VegasFlow(std::string idval, std::string dest, int data_size,
                     std::shared_ptr<Host> h, int winSize, double ts) :
                     Flow(idval, dest, data_size, h, winSize, ts) {
}


/**
 * Handles on UnackEvent for this flow.
 * 
 * @param unacked the unacknowledged packet
 * @param time the time at which the event is thrown.  This should be roughly
 * waitTime after the initial packet was sent.
 */
void VegasFlow::handleUnackEvent(std::shared_ptr<Packet> unacked, double time) {
    int seqNum = unacked->sequence_num;
    // If the packet has not been acknowledged...
    // Note that it is in fact possible to receive a legitimate unackEvent where
    // the seqNum is > the windowEnd, if we shrink the window size.  So, we must
    // check the upper bound and lower bound.
    if (seqNum >= windowStart && seqNum <= windowEnd) {
        // For go back N, just retransmit, as long as the packet is within
        // the window.
        auto resend = std::make_shared<Packet>(*unacked);
        resend->timestamp = time;
        sendAndQueueResend(resend, time, waitTime);
    }
}


/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  Note that this algorithm doesn't update the window
 * size.  Instead, our window size is updated by a recurring event called a
 * VegasUpdateEvent.
 *
 * @param p the ack packet
 * @param time the time at which the ack is received
 */
void VegasFlow::handleAck(std::shared_ptr<Packet> pkt, double time) {
    assert(pkt->ack);
    if (phase == DATA) {
        // Update the A, D, waitTime;
        RTT = time - pkt->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        waitTime = A + 4 * D + 0.01;
        logFlowRTT(time, RTT);
        minRTT = std::min(minRTT, RTT);

        int seqNum = pkt->sequence_num;

        if (seqNum == numPackets) {
            // We received an ack for a nonexistent packet.  I.e. we sent
            // the 100th packet (with seqNum 99), and this ack says "100", but
            // there is no 100th packet to send.  So we're done.

            // We're done sending packets.  Move the window outside of the bounds
            // of the packets.
            windowStart = seqNum;
            windowEnd = seqNum;

            phase = FIN;
            // We need to send a FIN to the other host.
            auto fin = std::make_shared<Packet>("FIN", destination,
                source, FIN_SIZE, false, -1, id, false, true, time);

            send(fin, time);
            return;
        }

        // If we're not done, update window and send packets.
        int windowSize = windowEnd - windowStart + 1;
        
        // Set windowStart to the current packet's sequence number
        windowStart = seqNum;
        windowEnd = std::min(seqNum + windowSize - 1, numPackets - 1);
        sendManyPackets(time);
    } // Otherwise, do nothing.
    logFlowWindowSize(time, windowEnd - windowStart + 1);
}


/**
 * Represent the Flow as a string.
 *
 * @return a string representing the Flow.
 */
std::string VegasFlow::toString() {

    std::stringstream fmt;
    std::stringstream setString;
    setString << "{unSentPackets: ";
    for (auto it = unSentPackets.begin(); it != unSentPackets.end(); it++) {
        setString << *it << ", ";
        if (*it % 20 == 0) { setString << "\n"; }
    }
    setString << "}";
    // std::string setElems = setString.str();
    std::string setElems = "";

    fmt << "{FLOW: id=" << id << ", source=" << source << ", destination=" << destination << ", numPackets=" << numPackets << ", waitTime=" << waitTime << ", " << "packetSize=" << packetSize << ", " << setElems << ", " << "windowStart=" << windowStart << ", windowEnd=" << windowEnd << "}";
    return fmt.str();
}


/**
 * Updates the window size for the VegasFlow.  This is called in response to
 * a VegasUpdateEvent being received by the Flow's Host.
 *
 * @param time the time at which the update happens.
 */
void VegasFlow::handleVegasUpdate(double time) {
    if (phase == DONE || phase == FIN) {
        return;
    }

    int windowSize = windowEnd - windowStart + 1;

    double testValue = (windowSize / minRTT) - (windowSize / RTT);

    if (testValue < (vegasConstAlpha / minRTT)) {
        windowSize++;
    }
    else if (testValue > (vegasConstBeta / minRTT)) {
        windowSize--;
    }

    windowEnd = windowSize + windowStart - 1;
    assert(windowEnd >= windowStart); // Make sure it's after or at windowStart

    sendManyPackets(time);

    // schedule another update
    auto update = std::make_shared<TCPVegasUpdateEvent>(source,
                                                        source,
                                                        time + waitTime,
                                                        id);
    host->addEventToLocalQueue(update);

    windowSize = windowEnd - windowStart + 1;
    logFlowWindowSize(time, windowSize);
}


/**
 * Called in response to a PacketEvent that is a SYN.  Starts data transfer.
 *
 * @param pkt the SYN packet.
 * @param time the time at which data transfer begins.
 */
void VegasFlow::respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) {
    assert(pkt->ack && pkt->syn);

    if (phase == SYN) {
        phase = DATA;

        // Send an ack for the SYNACK.  Don't queue a resend (as per algorithm
        // in book for connection establishment).
        auto ack = std::make_shared<Packet>("ACK",
                                            destination,
                                            source,
                                            ACK_SIZE,
                                            true, // It's an ack packet.
                                            -1, // Set seq_num to -1.
                                            id, // Id of the flow
                                            false, // Not a syn
                                            false, // Not a fin
                                            pkt->timestamp); // Timestamp here 
                                                             //shouldn't matter.  TODO make sure.
        send(ack, time);
        
        // Initialize the data flow.
        initialize(time);

        // TODO not all of these will be here, in the future.
        RTT = time - pkt->timestamp;
        A = RTT;
        D = RTT;
        waitTime = 4 * RTT + RTT;
        vegasConstAlpha = 1.5; // TODO move these, make respondToSynPacketEvent in Flow.cpp
        vegasConstBeta = 2.5;
        minRTT = RTT;

        auto vUpdate = std::make_shared<TCPVegasUpdateEvent>(source,  // TODO this isn't used...?
            source, time + waitTime, id);
        host->addEventToLocalQueue(vUpdate);
    }
    // If we're not in the SYN phase, do nothing.
}
