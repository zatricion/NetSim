#include "Flow.h"
#include "VegasFlow.h"
#include "Host.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"

/**
 * Constructor
 */
VegasFlow::VegasFlow(std::string idval, std::string dest, 
           int data_size, 
           std::shared_ptr<Host> h, int winSize, double ts) : Flow(idval, dest, data_size, h, winSize, ts) {

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
void VegasFlow::handleUnackEvent(std::shared_ptr<Packet> unacked, double time) {
    int WS = windowEnd - windowStart + 1;
    FILE_LOG(logDEBUG) << "BEFORE handleUnackEvent: time=" << time << ", WS=" << WS;
    assert(time > 0);
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

        auto resend = std::make_shared<Packet>(*unacked);
        resend->timestamp = time;
        FILE_LOG(logDEBUG) << "Called from handleUnackEvent; waitTime=" << waitTime;
        sendAndQueueResend(resend, time, waitTime);

    }
    else {
        FILE_LOG(logDEBUG1) << "No action on UnackEvent.";
    }

    WS = windowEnd - windowStart + 1;
    FILE_LOG(logDEBUG) << "AFTER handleUnackEvent: WS=" << WS;
}


/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  The CongestionAlgorithm decides.
 *
 * @param p the ack packet
 * @param time the time at which the ack is received
 */
void VegasFlow::handleAck(std::shared_ptr<Packet> pkt, double time) {
    int WS = windowEnd - windowStart + 1;
    FILE_LOG(logDEBUG) << "BEFORE handleAck: WS=" << WS;
    FILE_LOG(logDEBUG) << "Flow is handleAck";
    assert(pkt->ack);
    if (phase == DATA) {
        // Update the A, D, waitTime;
        RTT = time - pkt->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        waitTime = A + 4 * D;
        FILE_LOG(logDEBUG) << "RTT=" << RTT << ", A=" << A << ", D=" << D << ", waitTime=" << waitTime;
        logFlowRTT(time, RTT);

        minRTT = std::min(minRTT, RTT);
        

        int seqNum = pkt->sequence_num;

        if (seqNum == numPackets) {
            // We received an ack that requests a nonexistent packet.  I.e. we sent
            // the 100th packet (with seqNum 99), and this ack says "100", but there
            // is no 100th packet to send.  So we're done.

            // We're done sending packets.  Move the window outside of the bounds
            // of the packets.
            windowStart = seqNum;
            windowEnd = seqNum;

            FILE_LOG(logDEBUG1) << "DONE WITH DATA FLOW.";

            phase = FIN;
            // We need to send a FIN to the other host.
            auto fin = std::make_shared<Packet>("FIN", destination,
                source, FIN_SIZE, false, -1, id, false, true, time);

            host->send(fin, time);
            return;
        }

        // Send packets.
        int windowSize = windowEnd - windowStart + 1;
        FILE_LOG(logDEBUG) << "SIZE=" << windowSize << ", start=" << windowStart << ", end=" << windowEnd << ".";
        windowStart = seqNum;
        FILE_LOG(logDEBUG) << "START=" << windowStart << ", windowSize=" << windowSize << ", numPackets=" << numPackets << ".";
        windowEnd = std::min(seqNum + windowSize - 1, numPackets - 1);
        sendManyPackets(time);

        FILE_LOG(logDEBUG) << "SIZE=" << windowSize << ", start=" << windowStart << ", end=" << windowEnd << ".";
        FILE_LOG(logDEBUG) << "SIZE=" << windowSize << ", start=" << windowStart << ", end=" << windowEnd << ".";
        
        auto vUpdate = std::make_shared<TCPVegasUpdateEvent>(source,
                                                             source, time, id);
        host->addEventToLocalQueue(vUpdate);

    }
    // Otherwise, do nothing.

    logFlowWindowSize(time, windowEnd - windowStart + 1);
    WS = windowEnd - windowStart + 1;
    FILE_LOG(logDEBUG) << "AFTER handleAck: WS=" << WS;
}


/**
 * Represent the packet as a string.
 *
 * @return a string representing the packet
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


void VegasFlow::handleVegasUpdate(double time) {
    FILE_LOG(logDEBUG) << "VEGAS UPDATE EVENT alpha=" << vegasConstAlpha << ", beta=" << vegasConstBeta;
    if (phase == DONE || phase == FIN) {
        return;
    }

    int windowSize = windowEnd - windowStart + 1;
    FILE_LOG(logDEBUG) << "BEFORE: windowSize=" << windowSize;

    double testValue = (windowSize / minRTT) - (windowSize / RTT);

    FILE_LOG(logDEBUG) << "1:START=" << windowStart << ", END=" << windowEnd;
    // apply Vegas update
    if (testValue < (vegasConstAlpha / minRTT)) {
        FILE_LOG(logDEBUG) << "INC";
        windowSize++;
    }
    else if (testValue > (vegasConstBeta / minRTT)) {
        FILE_LOG(logDEBUG) << "DEC";
        windowSize--;
    }
    FILE_LOG(logDEBUG) << "2:START=" << windowStart << ", END=" << windowEnd;
    // Otherwise leave window size alone
    // Set the cap:

    windowEnd = windowSize + windowStart - 1;
    windowEnd = std::min(windowEnd, numPackets - 1); // Make sure it's before the end.
    windowEnd = std::max(windowEnd, windowStart); // Make sure it's after or at windowStart
    FILE_LOG(logDEBUG) << "3:START=" << windowStart << ", END=" << windowEnd;

    sendManyPackets(time);

//    // schedule another update
//    auto update = std::make_shared<TCPVegasUpdateEvent>(source,
//                                                        source,
//                                                        time + waitTime,
//                                                        id);
//    host->addEventToLocalQueue(update);

    windowSize = windowEnd - windowStart + 1;
    FILE_LOG(logDEBUG) << "AFTER: windowSize=" << windowSize;
    logFlowWindowSize(time, windowSize);
}

//void Flow::handleTimeout(int frCount, double time) {
    //(std::static_pointer_cast<TCPReno>(a))->handleTimeout(this, frCount, time);
//}

void VegasFlow::logFlowRTT(double time, float RTT) {
    FILE_LOG(logDEBUG) << "logFlowRTT: " << time << ", " << RTT;
    sim_plotter.logFlowRTT(id,
        std::make_tuple(time, RTT));
}

void VegasFlow::logFlowWindowSize(double time, int windowSize) {
    FILE_LOG(logDEBUG) << "logFlowWindowSize: " << time << ", " << (double) windowSize;
    sim_plotter.logFlowWindowSize(id,
        std::make_tuple(time, (double) windowSize));
}

void VegasFlow::openConnection(double time) {
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

        FILE_LOG(logDEBUG) << "Called from openConn; waitTime=" << waitTime;
    sendAndQueueResend(syn, time, waitTime);
}

void VegasFlow::respondToSynUnackEvent(double time) {
    // Check if synack has been received.
    if (phase == SYN) {
        FILE_LOG(logDEBUG) << "SYNACK not received.  Resending SYN.";
        openConnection(time);
    }
}

void VegasFlow::closeConnection(double time) {
    return;
}

void VegasFlow::respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) {
    // If we receive a SYN, it better be a SYNACK.  TODO rename to SynAckPacketEvent
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
        vegasConstAlpha = 1.0;
        vegasConstBeta = 3.0;
        minRTT = RTT;

        auto vUpdate = std::make_shared<TCPVegasUpdateEvent>(source, 
            source, time + waitTime, id);
        host->addEventToLocalQueue(vUpdate);
    }
    // If we're not in the SYN phase, do nothing.
}
