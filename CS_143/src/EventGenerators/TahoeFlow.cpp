// TODO something about the branch you pulled is not quite right.  This is not tahoe.  This is the old reno code.
#include "Flow.h"
#include "TahoeFlow.h"
#include "Host.h"
#include "CongestionAlg.h"
#include "TCPVegas.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"

/**
 * Constructor
 */
TahoeFlow::TahoeFlow(std::string idval, std::string dest, 
           int data_size, 
           std::shared_ptr<Host> h, int winSize, float ts) : Flow(idval, dest, data_size, h, winSize, ts) {
    tahoePhase = SLOWSTART;
    ssthresh = 999999;
    multiplicity = 0;
    winOverFlow = 0.0;
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
void TahoeFlow::handleUnackEvent(std::shared_ptr<Packet> unacked, float time) {
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

        ///////////////////////////////TAHSTART

//if (unacked->sequence_num != flow->windowStart) { return; }
    //if (unacked->timestamp < ignoreTime) { FILE_LOG(logDEBUG) << "UNACK Ignored."; return; }
    //ignoreTime = time;  // TODO should we?

    FILE_LOG(logDEBUG) << "HANDLINGUNACK: " << toString();
    FILE_LOG(logDEBUG) << "NUM=" << unacked->sequence_num;

    unacked->timestamp = time;
    // If in slowstart or cong av, go to slow start windowsize->1
    // change ssthresh to half previous win size.
    // 
    FILE_LOG(logDEBUG) << "TIMEOUT";
    tahoePhase = SLOWSTART;
    ssthresh = (windowEnd - windowStart + 1) / 2;
    // Set the window size to 1.
    //int oldWindowEnd = flow->windowEnd;
    windowEnd = windowStart;
    // TODO
    //for (int i = flow->windowEnd + 1; i <= oldWindowEnd; i++) {
        //flow->unSentPackets.insert(i);
    //}

    host->sendAndQueueResend(unacked, time, waitTime);
    FILE_LOG(logDEBUG) << "Handled UnackEvent.  Flow " << toString();









        /////////////////////////////TAHEND

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
void TahoeFlow::handleAck(std::shared_ptr<Packet> pkt, float time) {
    FILE_LOG(logDEBUG) << "Flow is handleAck";
    assert(pkt->ack);
    if (phase == DATA) {
        // Update the A, D, waitTime;
        float RTT = time - pkt->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        waitTime = A + 4 * D;
        FILE_LOG(logDEBUG) << "RTT=" << RTT << ", A=" << A << ", D=" << D << ", waitTime=" << waitTime;
        logFlowRTT(time, RTT);

        
    ///////////////////////TAH

    FILE_LOG(logDEBUG1) << "Handling an ack.  Packet " << pkt->toString();
    int seqNum = pkt->sequence_num;
    if (seqNum < windowStart) { return; }

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
        auto finEvent = std::make_shared<PacketEvent>(
            host->my_link->getID(), host->getID(), time, fin);
        host->addEventToLocalQueue(finEvent);
            
    }

    // Handle each case separately, depending on which phase of Reno we're in.

    if (tahoePhase == SLOWSTART) {
        FILE_LOG(logDEBUG) << "In SLOWSTART";
        int windowSize = windowEnd - windowStart + 1;
        // increase window size by 1, then readjust window and send packets.
        windowSize += 1;
        windowStart = seqNum;
        windowEnd = std::min(seqNum + windowSize - 1, numPackets - 1);
        sendManyPackets(time);
        if (windowSize > ssthresh) {
            FILE_LOG(logDEBUG) << "Entering CONGESTIONAVOIDANCE";
            tahoePhase = CONGESTIONAVOIDANCE;
        }
    }

    else if (tahoePhase == CONGESTIONAVOIDANCE) {
        if (seqNum == windowStart) {
            multiplicity += 1;
        }
        else {
            assert(seqNum > windowStart);
            multiplicity = 0;
        }

        if (multiplicity > 3) {
            // We have received triple acks.  Do a fast resend, then timeout.
            multiplicity = 0;

            FILE_LOG(logDEBUG) << "TRIPLICATE ACKS FOR " << pkt->toString();
            auto retrans = std::make_shared<Packet>(
                std::to_string(pkt->sequence_num),
                pkt->source,
                pkt->final_dest,
                DATA_PKT_SIZE,
                false,
                pkt->sequence_num,
                pkt->flowID,
                false, false, time);
            handleUnackEvent(retrans, time);
        }

        else {
            // We have not received triple acks.  We will send more packets,
            // after adjusting the window bounds.
            int windowSize = windowEnd - windowStart + 1;

            float wOverFlow = winOverFlow;

            assert(tahoePhase == CONGESTIONAVOIDANCE);
            FILE_LOG(logDEBUG) << "windowSize=" << windowSize;
            wOverFlow += 1.0 / (windowSize);
            windowSize += (int) wOverFlow;
            wOverFlow -= (int) wOverFlow;
            FILE_LOG(logDEBUG) << "windowSize=" << windowSize << ", winOverFlow=" << wOverFlow;

            winOverFlow = wOverFlow;
            windowStart = seqNum;
            windowEnd = std::min(seqNum + windowSize - 1, numPackets - 1);
            sendManyPackets(time);
        }
    } // end CONGESTIONAVOIDANCE block;

    // log the window size.
    logFlowWindowSize(time, windowEnd - windowStart + 1);







    ///////////////////////////////////////TAHEND
    }
    // Otherwise, do nothing.
}


/**
 * Represent the packet as a string.
 *
 * @return a string representing the packet
 */
std::string TahoeFlow::toString() {

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


void TahoeFlow::logFlowRTT(float time, float RTT) {
    FILE_LOG(logDEBUG) << "logFlowRTT: " << time << ", " << RTT;
    sim_plotter.logFlowRTT(id,
        std::make_tuple(time, RTT));
}

void TahoeFlow::logFlowWindowSize(float time, int windowSize) {
    FILE_LOG(logDEBUG) << "logFlowWindowSize: " << time << ", " << (float) windowSize;
    sim_plotter.logFlowWindowSize(id,
        std::make_tuple(time, (float) windowSize));
}

void TahoeFlow::openConnection(float time) {
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

void TahoeFlow::sendAndQueueResend(std::shared_ptr<Packet> pkt, float time, float delay) {
    host->sendAndQueueResend(pkt, time, delay);
}

void TahoeFlow::respondToSynUnackEvent(float time) {
    // Check if synack has been received.
    if (phase == SYN) {
        FILE_LOG(logDEBUG) << "SYNACK not received.  Resending SYN.";
        openConnection(time);
    }
}

void TahoeFlow::closeConnection(float time) {
    return;
}

void TahoeFlow::respondToSynPacketEvent(std::shared_ptr<Packet> pkt, float time) {
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
                                            false, // Not an ack
                                            pkt->timestamp); // Timestamp here 
                                                             //shouldn't matter.  TODO make sure.
        send(ack, time);
        
        // Initialize the data flow.
        initialize(time);

        // TODO not all of these will be here, in the future.
        float RTT = time - pkt->timestamp;
        A = RTT;
        D = RTT;
        waitTime = 4 * RTT + RTT;

    }
    // If we're not in the SYN phase, do nothing.
}

// TODO move to superclass.
void TahoeFlow::send(std::shared_ptr<Packet> pkt, float time) {
    host->send(pkt, time);
}
