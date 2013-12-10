#include "Flow.h"
#include "TahoeFlow.h"
#include "Host.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"
#include <climits>


/**
 * Constructor for a TahoeFlow object.
 *
 * @param idval the id of the flow.  IDs must be globally unique.
 * @param dest the destination of the flow.
 * @param data_size the data size of the flow
 * @param h a pointer to the sending end of the flow
 * @param winSize the initial window size of the flow (usually 1)
 * @param ts the time at which the flow begins
 */
TahoeFlow::TahoeFlow(std::string idval, std::string dest, int data_size,
                     std::shared_ptr<Host> h, int winSize, double ts) :
                     Flow(idval, dest, data_size, h, winSize, ts) {
    tahoePhase = SLOWSTART;
    ssthresh = INT_MAX;
    multiplicity = 0;
    winOverFlow = 0.0;
    validUnackTime = 0.0;
}


/**
 * This method is called when the TahoeFlow receives an UnackEvent.  First, the
 * TahoeFlow determines if the Packet has been acknowledged or not.  Then,
 * it times out if the Packet has not been acknowledged.
 * 
 * @param unacked the potentially unacknowledged packet
 * @param time the time at which the event is thrown.  This should be roughly
 * waitTime after the initial packet was sent.
 */
void TahoeFlow::handleUnackEvent(std::shared_ptr<Packet> unacked, double time) {
    int seqNum = unacked->sequence_num;
    
    // If we have a timeout, we want to effectively clear the Host's eventHeap
    // of all UnackEvents for this Flow, because we don't want to repeat the
    // timeout.  As a proxy for dequeueing these events, we keep a variable
    // called validUnackTime.  The variable remembers when the last timeout
    // occurred.  If the next UnackEvent was generated before validUnackTime,
    // we ignore the next UnackEvent.
    //
    // We know that the timestamp of the Packet in the UnackEvent is the time
    // at which the UnackEvent is created.  So this 'if' statement tests whether
    // the UnackEvent was generated before the most recent timeout.
    if (unacked->timestamp < validUnackTime) {
        return;
    }

    // If the packet has not been acknowledged...
    // Note that it is in fact possible to receive a legitimate unackEvent where
    // the seqNum is > the windowEnd, if we shrink the window size.  So, we must
    // check the upper bound and lower bound.
    if (seqNum >= windowStart && seqNum <= windowEnd) {
        // Update validUnackTime.
        validUnackTime = time;

        // Since we are ignoring UnackEvents for Packets, we need to add those
        // Packets to unSentPackets, so that they are later recent.  Otherwise,
        // we may never actually send them again.
        for (int i = windowStart + 1; i <= windowEnd; i++) {
            unSentPackets.insert(i);
        }

        // Create a new packet to send.  It is almost identical to unacked.
        auto sendMe = std::make_shared<Packet>(*unacked);
        sendMe->timestamp = time;
        sendMe->sequence_num = windowStart;

        tahoePhase = SLOWSTART;
        ssthresh = (windowEnd - windowStart + 1) / 2;

        // Set the window size to 1.
        windowEnd = windowStart;

        sendAndQueueResend(sendMe, time, waitTime);
    }
}


/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  The CongestionAlgorithm decides.
 *
 * @param p the ack packet
 * @param time the time at which the ack is received
 */
void TahoeFlow::handleAck(std::shared_ptr<Packet> pkt, double time) {
    assert(pkt->ack);

    // Only take an action is phase == DATA.  Otherwise, there's no point, since
    // data flow is over.
    if (phase == DATA) {
        int seqNum = pkt->sequence_num;

        // If the Packet's sequence number is below the windowStart, we don't
        // need to do anything.
        if (seqNum < windowStart) {
            return;
        }

        // We want to ignore acks that come before the validUnackTime.
        // This is because they can affect the multiplicity unfairly and cause
        // the flow to timeout unnecessarily.
        if (pkt->timestamp < validUnackTime) {
            return;
        }

        // Update the A, D, waitTime, according to algorithm explained in Flow.
        double RTT = time - pkt->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        waitTime = 1.5 * A + 4 * D + .01; // If you just use A, there are some packets that BARELY don't make it.
        // Seems wrong to use just A.  //TODO better comments.

        FILE_LOG(logDEBUG) << "RTT=" << RTT << ", A=" << A << ", D=" << D << ", waitTime=" << waitTime;
        logFlowRTT(time, RTT);

       
        FILE_LOG(logDEBUG1) << "Handling an ack.  Packet " << pkt->toString();
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

                double wOverFlow = winOverFlow;

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

        logFlowWindowSize(time, windowEnd - windowStart + 1);
    }
    // Otherwise (if phase is not data), do nothing.
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
    std::string setElems = setString.str();// setString.str();

    fmt << "{FLOW: id=" << id << ", source=" << source << ", destination=" << 
        destination << ", numPackets=" << numPackets << ", waitTime=" << 
        waitTime << ", " << "packetSize=" << packetSize << ", " << setElems <<
        ", " << "windowStart=" << windowStart << ", windowEnd=" << windowEnd << "}";
    return fmt.str();
}


void TahoeFlow::closeConnection(double time) {
    return;
}


void TahoeFlow::respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) {
    // If we receive a SYN, it better be a SYNACK.
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
                                            pkt->timestamp);
        send(ack, time);
        
        // Initialize the data flow.
        initialize(time);

        double RTT = time - pkt->timestamp;
        A = RTT;
        D = RTT;
        waitTime = 4 * RTT + RTT;
    }
    // If we're not in the SYN phase, do nothing.
}
