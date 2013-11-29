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
    
    FILE_LOG(logDEBUG) << "DEFAULT CONSTRUCTING FLOW.";
    host = NULL;
    id = "id";
    source = "source";
    destination = "destination";
    a = NULL;
    numPackets = 0;
    //acknowledgedPackets = std::set<int>();
    std::queue<Packet> flow;
    windowSize = -1;
    timestamp = -1.0;
    numAcked = 0;
    phase = SYN;
    unSentPackets = std::set<int>();
    renoPhase = SLOWSTART;
    ssthresh = 20; //TODO pick this.
}

/**
 * Initializes the data flow.  Calls the congestion algorithm to see what to
 * do.
 */
void Flow::initialize() {
    FILE_LOG(logDEBUG) << "Initializing data stream from flow with id=" << id;
    FILE_LOG(logDEBUG) << "jjj" << unSentPackets.size();
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
    unAckedPackets = std::set<int>();
    windowSize = winSize;
    timestamp = ts;
    ssthresh = 20;
    // TODO this CANNOT be merged with unAckedPackets, it serves a separate
    // purpose.  Don't delete this.  For real, don't delete it.
    // Maps a packet's sequence number to the number of acks received for that
    // packet.
    multiplicityOfAcksReceived = std::map<int, int>();
    
    // TODO this should be calculated by the algorithm, or something.  For
    // now, just use a default.
    waitTime = 500.0;
    numAcked = 0;
    phase = SYN;
    unSentPackets = std::set<int>();
    for (int i = 0; i < numPackets; i++) {
        unSentPackets.insert(i);
    }
    FILE_LOG(logDEBUG) << "USPSize=" << unSentPackets.size();
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
        FILE_LOG(logDEBUG) << "Packet was:" << unacked->toString();
        a->handleUnackEvent(this, unacked, time);
    }
    else {
        FILE_LOG(logDEBUG) << "Ack had already been received.";
        FILE_LOG(logDEBUG) << "unSentPackets.size()=" << unSentPackets.size();
    }
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
    std::stringstream setString;
    setString << "{unSentPackets: ";
    for (auto it = unSentPackets.begin(); it != unSentPackets.end(); it++) {
        setString << *it << ", ";
        if (*it % 20 == 0) { setString << "\n"; }
        //std::cout << "read";
        //std::cout << *it;
    }
    setString << "}";
    std::string setElems = setString.str();

    std::stringstream unAckedString; 
    unAckedString << "{unAckedPackets: ";
    for (auto it = unAckedPackets.begin(); it != unAckedPackets.end(); it++) {
        unAckedString << *it << ", ";
        if (*it % 20 == 0) { unAckedString << "\n"; }
    }
    unAckedString << "}";
    std::string unAckedElems = unAckedString.str();

    // TODO get the unAckedPackets to print nicely.
    fmt << "{FLOW: id=" << id << ", source=" << source << ", destination=" << destination << ", numPackets=" << numPackets << ", waitTime=" << waitTime << ", windowSize=" << windowSize << ", packetSize=" << packetSize << ", timestamp=" << timestamp << ", numAcked=" << numAcked << ", " << setElems << ", " << unAckedElems << "}";
    return fmt.str();
}
