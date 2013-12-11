#include "VegasFlow.h"


/**
 * Constructor for instance of Vegas algorithm.
 *
 * @param idval the unique id of the flow.
 * @param dest the destination of the flow.
 * @param data_size the size of the data transfer.
 * @param h the host on which the flow is generated.
 * @param win_size the initial window size (usually 1).
 * @param ts the timestamp at which the flow is generated.
 */
VegasFlow::VegasFlow(std::string idval, std::string dest, int data_size,
                     std::shared_ptr<Host> h, int win_size, double ts) :
                     Flow(idval, dest, data_size, h, win_size, ts) {
}


/**
 * Handles on UnackEvent for this flow.
 * 
 * @param unacked the unacknowledged packet
 * @param time the time at which the event is thrown.  This should be roughly
 * wait_time after the initial packet was sent.
 */
void VegasFlow::handleUnackEvent(std::shared_ptr<Packet> unacked, double time) {
    int seq_num = unacked->sequence_num;
    // If the packet has not been acknowledged...
    // Note that it is in fact possible to receive a legitimate unackEvent where
    // the seq_num is > the window_end, if we shrink the window size.  So, we 
    // check the upper bound and lower bound.
    if (seq_num >= window_start && seq_num <= window_end) {
        // For go back N, just retransmit, as long as the packet is within
        // the window.
        auto resend = std::make_shared<Packet>(*unacked);
        resend->timestamp = time;
        sendAndQueueResend(resend, time, wait_time);
    }
}


/**
 * When an ack is received from the flow's destination, this method is called,
 * to determine what to do.  Note that this algorithm doesn't update the window
 * size.  Instead, our window size is updated by a recurring event called a
 * VegasUpdateEvent.
 *
 * @param pkt the ack packet
 * @param time the time at which the ack is received
 */
void VegasFlow::handleAck(std::shared_ptr<Packet> pkt, double time) {
    assert(pkt->ack);
    if (phase == DATA) {
        // Update the A, D, wait_time;
        RTT = time - pkt->timestamp;
        A = A * (1.0 - b) + b * RTT;
        D = (1.0 - b) * D + b * abs(RTT - A);
        updateWaitTime();
        logFlowRTT(time, RTT);
        minRTT = std::min(minRTT, RTT);

        int seq_num = pkt->sequence_num;

        if (seq_num == num_packets) {
            // We received an ack for a nonexistent packet.  I.e. we sent
            // the 100th packet (with seq_num 99), and this ack says "100", but
            // there is no 100th packet to send.  So we're done.

            // We're done sending packets.  Move the window outside of the bounds
            // of the packets.
            window_start = seq_num;
            window_end = seq_num;

            phase = FIN;
            // We need to send a FIN to the other host.
            auto fin = std::make_shared<Packet>("FIN", destination,
                source, FIN_SIZE, false, -1, id, false, true, time);

            send(fin, time);
            return;
        }

        // If we're not done, update window and send packets.
        int window_size = window_end - window_start + 1;
        
        // Set window_start to the current packet's sequence number
        window_start = seq_num;
        window_end = std::min(seq_num + window_size - 1, num_packets - 1);
        sendManyPackets(time);
    } // Otherwise, do nothing.
    logFlowWindowSize(time, window_end - window_start + 1);
}


/**
 * Represent the Flow as a string.
 *
 * @return a string representing the Flow.
 */
std::string VegasFlow::toString() {

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
 * Updates the window size for the VegasFlow.  This is called in response to
 * a VegasUpdateEvent being received by the Flow's Host.
 *
 * @param time the time at which the update happens.
 */
void VegasFlow::handleVegasUpdate(double time) {
    if (phase == DONE || phase == FIN) {
        return;
    }

    int window_size = window_end - window_start + 1;

    double test_value = (window_size / minRTT) - (window_size / RTT);

    if (test_value < (vegas_alpha / minRTT)) {
        window_size++;
    }
    else if (test_value > (vegas_beta / minRTT)) {
        window_size--;
    }

    window_end = window_size + window_start - 1;
    assert(window_end >= window_start);

    sendManyPackets(time);

    // schedule another update
    auto update = std::make_shared<TCPVegasUpdateEvent>(source,
                                                        source,
                                                        time + wait_time,
                                                        id);
    host->addEventToLocalQueue(update);

    window_size = window_end - window_start + 1;
    logFlowWindowSize(time, window_size);
}


/**
 * Called in response to a PacketEvent that is a SYN.  Starts data transfer.
 *
 * @param pkt the SYN packet.
 * @param time the time at which SYN is received.
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
                                            pkt->timestamp);

        send(ack, time);
        
        // Initialize the data flow.
        initialize(time);

        RTT = time - pkt->timestamp;
        A = RTT;
        D = RTT;
        updateWaitTime();
        // Our choices of constants for Vegas.
        vegas_alpha = 1.5;
        vegas_beta = 2.5;
        minRTT = RTT;

        auto vUpdate = std::make_shared<TCPVegasUpdateEvent>(source, source,
                                                             time + wait_time,
                                                             id);
        host->addEventToLocalQueue(vUpdate);
    } // If we're not in the SYN phase, do nothing.
}


/**
 * Updates the wait_time for Vegas.  A slightly modified version of A + 4D is
 * used, because D rapidly becomes very small.
 */
void VegasFlow::updateWaitTime() {
    wait_time = A + 4 * D + .01;
}
