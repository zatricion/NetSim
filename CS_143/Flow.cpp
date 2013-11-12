#include "Flow.h"

// TODO numPckts deduced from data size and packet size.
// TODO add window size.  Can move this into the algorithm later if desired.
// TODO need start timestamp.
Flow::Flow(std::string id, std::string src, std::string dest,
           CongestionAlgorithm *alg, int numPckts, int packetSize, Host *host,
           int winSize, float ts) {
    windowSize = winSize;
    timeStamp = ts;
    source = src;
    destination = dest;
    a = alg;
    std::unordered_set<int> acknowledgedPackets;
    // TODO this should be calculated by the algorithm, or something.  For
    // now, just use a default.
    waitTime = 500.0;
    
    std::queue<Packet> flow;
    for (int count = 0; count < numPackets; count++) {
        std::string pack_id = this->id + std::to_string(count);
        Packet new_packet(pack_id, dest, host->getId(), packetSize, false,
                          false, count);
        flow.push(new_packet);
    }
    a.call(this);
}

void Flow::handleUnackEvent(Packet unacked, float time) {
    // TODO absorb ALL this logic into the CongestionAlgorithm
    int seqNum = unacked.sequence_num;
    if (!unacknowledgedPackets.count(seqNum)) {
        // We didn't find the item in the set of acknowledged packets.
        // We must resend the packet.

        // Old code (belongs in a.call()) // TODO
        //Packet new_pack = unack_event->packet;
        //sendPacket(new_pack, unack_event->eventTime());

        // TODO call the CongestionAlgorithm, so it can update.
        // All we have to do is call the congestion algorithm.
        a.call(this, unacked, time);
    }
    // else, there's nothing to do.
}

// Handle an ack received from the flow's destination.
void Flow::handleAck(Packet p, float time) { // TODO makes more sense if arg is an event, not packet.
    a.call(this, p, time);
}
