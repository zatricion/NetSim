#include "Flow.h"
#include "Host.h"
#include "CongestionAlg.h"
#include "TCPReno.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"

/**
 * Initializes the data flow.  Calls the congestion algorithm to see what to
 * do.
 */
void Flow::initialize(float time) {
    FILE_LOG(logDEBUG1) << "Initializing data stream from flow with id=" << id;
    a->initialize(this, time);
}


/**
 * Constructor
 */
Flow::Flow(std::string idval, std::string dest, 
           std::shared_ptr<CongestionAlg> alg, int data_size, 
           std::shared_ptr<Host> h, int winSize, float ts) {
    timestamp = ts;
    host = h;
    id = idval;
    source = h->getID();
    destination = dest;
    a = alg;
    numPackets = ceil(1.0 * data_size / DATA_PKT_SIZE);
    ssthresh = 20;
    multiplicity = 0;
    waitTime = 500.0;
    phase = SYN;
    unSentPackets = std::set<int>();
    for (int i = 0; i < numPackets; i++) {
        unSentPackets.insert(i);
    }
    windowStart = 0;
    windowEnd = winSize - 1;
    cavCount = 0;
    frCount = 0;
    fastWindowEnd = -1;
    A = waitTime;
    D = waitTime;
    b = .5;
}


/**
 * When a packet has not been acknowledged after waitTime, this method is
 * called, to determine what to do.  Another copy of the packet will be sent,
 * then the CongestionAlgorithm will update the fields.
 * 
 * @param unacked the unacknowledged packet
 * @param time the time at which the event is thrown.  This should be roughly
 * waitTime after the initial packet was sent.
 */
void Flow::handleUnackEvent(std::shared_ptr<Packet> unacked, float time) {
    int seqNum = unacked->sequence_num;
    // If the packet has not been acknowledged...
    // Note that it is in fact possible to receive a legitimate unackEvent where
    // the seqNum is > the windowEnd, if we shrink the window size.  So, we must
    // check the upper bound and lower bound.
    if (seqNum >= windowStart && seqNum <= windowEnd) {
        // For go back N, just retransmit, as long as the packet is within
        // the window.
        FILE_LOG(logDEBUG1) << "Packet unacknowledged.  Better resend.";
        FILE_LOG(logDEBUG1) << "Packet was:" << unacked->toString() <<
            ", time=" << time;
        a->handleUnackEvent(this, unacked, time);
    }
    else {
        FILE_LOG(logDEBUG1) << "No action on UnackEvent.";
    }
}


/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  The CongestionAlgorithm decides.
 *
 * @param p the ack packet
 * @param time the time at which the ack is received
 */
void Flow::handleAck(std::shared_ptr<Packet> p, float time) {
    FILE_LOG(logDEBUG1) << "Ack received by flow with id=" << id;
    assert(p->ack);
    if (phase == DATA) {
        // Update the A, D, waitTime;
        float RTT = time - p->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        waitTime = A + 4 * D;
        FILE_LOG(logDEBUG) << "RTT=" << RTT << ", A=" << A << ", D=" << D << ", waitTime=" << waitTime;
        a->handleAck(this, p, time);
    }
    else if (phase == FIN || phase == DONE) {
            FILE_LOG(logDEBUG1) << "Received a non-fin ack when in the FIN" <<
                " or DONE phase.  Do nothing.";
    }
    else {
        assert(false); // SYNACKs should not call this function.
    }
}


/**
 * Represent the packet as a string.
 *
 * @return a string representing the packet
 */
std::string Flow::toString() {

    std::stringstream fmt;
    std::stringstream setString;
    setString << "{unSentPackets: ";
    for (auto it = unSentPackets.begin(); it != unSentPackets.end(); it++) {
        setString << *it << ", ";
        if (*it % 20 == 0) { setString << "\n"; }
    }
    setString << "}";
    std::string setElems = setString.str();

    fmt << "{FLOW: id=" << id << ", source=" << source << ", destination=" << destination << ", numPackets=" << numPackets << ", waitTime=" << waitTime << ", " << "packetSize=" << packetSize << ", " << setElems << ", " << "windowStart=" << windowStart << ", windowEnd=" << windowEnd << "}";
    return fmt.str();
}

void Flow::handleRenoUpdate(int cavCount, float time) {
    (std::static_pointer_cast<TCPReno>(a))->handleRenoUpdate(this, cavCount, time);
}

void Flow::handleTimeout(int frCount, float time) {
    (std::static_pointer_cast<TCPReno>(a))->handleTimeout(this, frCount, time);
}
