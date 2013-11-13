#include "Flow.h"
#include "Host.h"
#include "CongestionAlg.h"

static const int DATA_PKT_SIZE = 100000;

// TODO numPckts deduced from data size and packet size.
// TODO add window size.  Can move this into the algorithm later if desired.
// TODO need start timestamp.
Flow::Flow(std::string idval, std::string src, std::string dest,
           CongestionAlg *alg, int data_size, Host *h,
           int winSize, float ts) {
    host = h;
    id = idval;
    source = src;
    destination = dest;
    a = alg;
    numPackets = data_size / DATA_PKT_SIZE;
    std::unordered_set<int> acknowledgedPackets;
    std::queue<Packet> flow;
    windowSize = winSize;
    timeStamp = ts;
    
    // TODO this should be calculated by the algorithm, or something.  For
    // now, just use a default.
    waitTime = 500.0;
    
    std::map<std::string, std::vector<std::string> > table;
    
    for (int count = 0; count < numPackets; count++) {
        std::string pack_id = this->id + std::to_string(count);
        Packet new_packet(pack_id, destination, source, packetSize, false,
                          false, false, table, count);
        flow.push(new_packet);
    }
    a->initialize(this);
}

void Flow::handleUnackEvent(Packet unacked, float time) {
    // TODO absorb ALL this logic into the CongestionAlgorithm
    int seqNum = unacked.sequence_num;
    if (!acknowledgedPackets.count(seqNum)) {
        // We didn't find the item in the set of acknowledged packets.
        // We must resend the packet.

        // Old code (belongs in a.call()) // TODO
        //Packet new_pack = unack_event->packet;
        //sendPacket(new_pack, unack_event->eventTime());

        // TODO call the CongestionAlgorithm, so it can update.
        // All we have to do is call the congestion algorithm.
        a->handleUnackEvent(this, unacked, time);
    }
    // else, there's nothing to do.
}

// Handle an ack received from the flow's destination.
void Flow::handleAck(Packet p, float time) { // TODO makes more sense if arg is an event, not packet.
    a->handleAck(this, p, time);
}
