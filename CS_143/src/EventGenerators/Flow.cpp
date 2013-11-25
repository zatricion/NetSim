#include "Flow.h"
#include "Host.h"
#include "CongestionAlg.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"

/**
 * Default constructor.
 */
Flow::Flow(){
    
    host = NULL;
    id = "id";
    source = "source";
    destination = "destination";
    a = NULL;
    numPackets = 0;
    std::set<int> acknowledgedPackets;
    std::queue<Packet> flow;
    windowSize = -1;
    timestamp = -1.0;
    numAcked = 0;
    packetSize = DATA_PKT_SIZE;
}

/**
 * Initializes the data flow.  Calls the congestion algorithm to see what to
 * do.
 */
void Flow::initialize() {
    FILE_LOG(logDEBUG) << "Initializing data stream from flow with id=" << id;
    a->initialize(this);
}

/**
 * Constructor
 */
Flow::Flow(std::string idval, std::string dest, 
           std::shared_ptr<CongestionAlg> alg, int data_size, 
           std::shared_ptr<Host> h, int winSize, float ts) {
    host = h;
    id = idval;
    source = h->getID();
    destination = dest;
    a = alg;
    numPackets = ceil(1.0 * data_size / DATA_PKT_SIZE);
    std::set<int> unAckedPackets;
    windowSize = winSize;
    timestamp = ts;
    
    // TODO this should be calculated by the algorithm, or something.  For
    // now, just use a default.
    waitTime = 500.0;
    numAcked = 0;
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
    FILE_LOG(logDEBUG4) << "Flow is handling an UnackEvent.  FlowID=" << id;

    int seqNum = unacked->sequence_num;
    // If the packet has not been acknowledged...
    if (unAckedPackets.count(seqNum)) {
        FILE_LOG(logDEBUG) << "Packet unacknowledged.  Better resend.";
        a->handleUnackEvent(this, unacked, time);
    }
    FILE_LOG(logDEBUG) << "Ack had already been received.";
    // Otherwise, do nothing.
}

/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  The CongestionAlgorithm decides.
 *
 * @param p the ack packet
 * @param time the time at which the ack is received
 */
void Flow::handleAck(std::shared_ptr<Packet> p, float time) {
    FILE_LOG(logDEBUG) << "Ack received by flow with id=" << id;
    assert(p->ack);
    a->handleAck(this, p, time);
}

/**
 * Represent the packed as a string.
 *
 * @return a string representing the packet
 */
std::string Flow::toString() {
    std::stringstream fmt;
    // TODO get the unAckedPackets to print nicely.
    fmt << "{FLOW: id=" << id << ", source=" << source << ", destination=" << destination << ", numPackets=" << numPackets << ", waitTime=" << waitTime << ", windowSize=" << windowSize << ", packetSize=" << packetSize << ", timestamp=" << timestamp << ", numAcked=" << numAcked << "}";
    return fmt.str();
}
