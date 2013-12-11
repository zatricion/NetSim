#include "TahoeFlow.h"


/**
 * Constructor for a TahoeFlow object.
 *
 * @param idval the id of the flow.  IDs must be globally unique.
 * @param dest the destination of the flow.
 * @param data_size the data size of the flow
 * @param h a pointer to the sending end of the flow
 * @param win_size the initial window size of the flow (usually 1)
 * @param ts the time at which the flow begins
 */
TahoeFlow::TahoeFlow(std::string idval, std::string dest, int data_size,
                     std::shared_ptr<Host> h, int win_size, double ts) :
                     Flow(idval, dest, data_size, h, win_size, ts) {
    tahoe_phase = SLOWSTART;
    ssthresh = INT_MAX;
    multiplicity = 0;
    win_over_flow = 0.0;
    valid_unack_time = 0.0;
}


/**
 * This method is called when the TahoeFlow receives an UnackEvent.  First, the
 * TahoeFlow determines if the Packet has been acknowledged or not.  Then,
 * it times out if the Packet has not been acknowledged.
 * 
 * @param unacked the potentially unacknowledged packet
 * @param time the time at which the event is thrown.  This should be roughly
 * wait_time after the initial packet was sent.
 */
void TahoeFlow::handleUnackEvent(std::shared_ptr<Packet> unacked, double time) {
    int seq_num = unacked->sequence_num;
    
    // If we have a timeout, we want to effectively clear the Host's event_heap
    // of all UnackEvents for this Flow, because we don't want to repeat the
    // timeout.  As a proxy for dequeueing these events, we keep a variable
    // called valid_unack_time.  The variable remembers when the last timeout
    // occurred.  If the next UnackEvent was generated before valid_unack_time,
    // we ignore the next UnackEvent.
    //
    // We know that the timestamp of the Packet in the UnackEvent is the time
    // at which the UnackEvent is created.  So this 'if' statement tests whether
    // the UnackEvent was generated before the most recent timeout.
    if (unacked->timestamp < valid_unack_time) {
        return;
    }

    // If the packet has not been acknowledged...
    // Note that it is in fact possible to receive a legitimate unackEvent where
    // the seq_num is > the window_end, if we shrink window size.  So, we must
    // check the upper bound and lower bound.
    if (seq_num >= window_start && seq_num <= window_end) {
        // Update valid_unack_time.
        valid_unack_time = time;

        // Since we are ignoring UnackEvents for Packets, we need to add those
        // Packets to unsent_packets, so that they are later recent.  Otherwise,
        // we may never actually send them again.
        for (int i = window_start + 1; i <= window_end; i++) {
            unsent_packets.insert(i);
        }

        // Create a new packet to send.  It is almost identical to unacked.
        auto sendMe = std::make_shared<Packet>(*unacked);
        sendMe->timestamp = time;
        sendMe->sequence_num = window_start;

        tahoe_phase = SLOWSTART;
        ssthresh = (window_end - window_start + 1) / 2;

        // Set the window size to 1.
        window_end = window_start;

        sendAndQueueResend(sendMe, time, wait_time);
    }
}


/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  The CongestionAlgorithm decides.
 *
 * @param pkt the ack packet
 * @param time the time at which the ack is received
 */
void TahoeFlow::handleAck(std::shared_ptr<Packet> pkt, double time) {
    assert(pkt->ack);

    // Only take an action is phase == DATA.  Otherwise, there's no point, since
    // data flow is over.
    if (phase == DATA) {
        int seq_num = pkt->sequence_num;

        // If the Packet's sequence number is below the window_start, we don't
        // need to do anything.
        if (seq_num < window_start) {
            return;
        }

        // We want to ignore acks that come before the valid_unack_time.
        // This is because they can affect the multiplicity unfairly and cause
        // the flow to timeout unnecessarily.
        if (pkt->timestamp < valid_unack_time) {
            return;
        }

        // Update the A, D, wait_time, according to algorithm explained in Flow.
        double RTT = time - pkt->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        updateWaitTime();

        logFlowRTT(time, RTT);
       
        if (seq_num < window_start) { return; }

        if (seq_num == num_packets) {
            // We received an ack that requests a nonexistent packet.  I.e. we sent
            // the 100th packet (with seq_num 99), and this ack says "100", but there
            // is no 100th packet to send.  So we're done.

            // We're done sending packets.  Move the window outside of the bounds
            // of the packets.
            window_start = seq_num;
            window_end = seq_num;

            phase = FIN;
            // We need to send a FIN to the other host.
            auto fin = std::make_shared<Packet>("FIN", destination,
                source, FIN_SIZE, false, -1, id, false, true, time);
            auto finEvent = std::make_shared<PacketEvent>(
                host->my_link->getID(), host->getID(), time, fin);
            host->addEventToLocalQueue(finEvent);
            
        }

        if (tahoe_phase == SLOWSTART) {
            FILE_LOG(logDEBUG) << "In SLOWSTART";
            int window_size = window_end - window_start + 1;
            // increase window size by 1, then readjust window and send packets.
            window_size += 1;
            window_start = seq_num;
            window_end = std::min(seq_num + window_size - 1, num_packets - 1);
            sendManyPackets(time);
            if (window_size > ssthresh) {
                FILE_LOG(logDEBUG) << "Entering CONGESTIONAVOIDANCE";
                tahoe_phase = CONGESTIONAVOIDANCE;
            }
        }

        else if (tahoe_phase == CONGESTIONAVOIDANCE) {
            if (seq_num == window_start) {
                multiplicity += 1;
            }
            else {
                assert(seq_num > window_start);
                multiplicity = 0;
            }

            if (multiplicity > 3) {
                // We have received triple duplicate acks.
                multiplicity = 0;

                auto retrans = std::make_shared<Packet>(
                    std::to_string(pkt->sequence_num),
                    pkt->source,
                    pkt->final_dest,
                    DATA_PKT_SIZE,
                    false,
                    pkt->sequence_num,
                    pkt->flowID,
                    false, false, time);
                // To timeout, just call handleUnackEvent!
                handleUnackEvent(retrans, time);
            }

            else {
                // We have not received triple acks.  We will send more packets,
                // after adjusting the window bounds.
                int window_size = window_end - window_start + 1;

                assert(tahoe_phase == CONGESTIONAVOIDANCE);
                win_over_flow += 1.0 / (window_size);
                window_size += (int) win_over_flow;
                win_over_flow -= (int) win_over_flow;

                window_start = seq_num;
                window_end = std::min(seq_num + window_size - 1, num_packets - 1);
                sendManyPackets(time);
            }
        } // end CONGESTIONAVOIDANCE block;
        logFlowWindowSize(time, window_end - window_start + 1);
    } // end phase == DATA block. If phase is not data, do nothing.
}


/**
 * Represent the Flow as a string.
 *
 * @return a string representing the Flow.
 */
std::string TahoeFlow::toString() {
    std::stringstream fmt;
    std::stringstream set_string;
    set_string << "{unsent_packets: ";
    for (auto it = unsent_packets.begin(); it != unsent_packets.end(); it++) {
        set_string << *it << ", ";
        if (*it % 20 == 0) { set_string << "\n"; }
    }
    set_string << "}";
    std::string set_elems = set_string.str();

    fmt << "{FLOW: id=" << id << ", source=" << source << ", destination=" << 
        destination << ", num_packets=" << num_packets << ", wait_time=" << 
        wait_time << ", " << ", " << set_elems << ", " << "window_start=" <<
        window_start << ", window_end=" << window_end << "}";
    return fmt.str();
}


/**
 * Called when a SYN PacketEvent for this flow reaches the host that the flow
 * lives on.  It ends the SYN handshake and starts the data flow.
 *
 * @param pkt the Packet from the PacketEvent.
 * @param time the time at which the PacketEvent arrives.
 */
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
        updateWaitTime();
    }
    // If we're not in the SYN phase, do nothing.
}


/**
 * Updates the wait time using a simple formula.
 * Uses a slightly modified version of the formula A + 4 * D.
 * Using A + 4 * D produces much more packet loss, because D becomes
 * small very quickly in the simulation, making the wait_time variable
 * very unforgiving.
 */
void TahoeFlow::updateWaitTime() {
    wait_time = 1.5 * A + 4 * D + .01;
}
