#include "CongestionAlg.h"

// TODO:
/*
 * CongestionAlg will be an abstract class from which all other algorithms
 * inherit.  For now, we create a "dumb" sample algorithm.
 */

// Initial call to the Congestion Algorithm.  Occurs only when the Flow object
// is first created.
void CongestionAlg::call(Flow *flow) {
    int windowSize = flow->windowSize;
    Host *host = flow->host;
    // At the outset, add a number of events equal to the window size.
    for (int i = 0; i < windowSize; i++) {
        Packet p = new Packet(std::string(i), flow->destination, flow->source,
                              flow->packetSize, false, false, i);
        // TODO the timestamps will all be the same, unless we add 
        // some value.  This should be i times the link delay, but we need
        // more changes to get that to work anyway.
        
        // Add the event.
        PacketEvent e = new PacketEvent(p, flow->link.getId(), flow->source, flow->timeStamp + i);
        host->addEvent(e);

        // Add an event to fire when we are tired of waiting.
        UnackEvent ue = new UnackEvent(p, flow->link.getId(), flow->source, flow->timeStamp + i + flow->waitTime);
        host->addEvent(ue);
    }
}

// Called when an event was not acknowledged.  Must update fields, resend the
// event.
// TODO need eventTime()
void CongestionAlg::call(Flow *flow, Packet unacked, float time) {
    //Packet unacked = e->packet;
    PacketEvent e = new PacketEvent(unacked, flow->dest, flow->source, time);
    // TODO also destroy the UnackEvent;
    // TODO most algorithms will update window size, etc.
}

// Called when the flow handles an ack.
void CongestionAlg::call(Flow *flow, Packet pkt, float time) {
    Host *host = flow->host;
    flow->acknowledgedPackets.emplace(pkt.sequence_num);
    
    // After acknowledging the packet, check if there are more packets to send.
    if (flow->acknowledgedPackets.size() == flow->numPackets) {
        // We are finished.  TODO destroy the flow object.
        continue;
    }

    else {
        // Update the window size.  TODO

        // TODO this is super ghetto.  Shouldn't've used an unordered set.
        // Also this isn't a real algorithm so whatever.
        int lowestUnacked = flow->numPackets;
        for (int i = 0; i < flow->numPackets; i++) {

            // TODO: in real life, we want to send the lowest numbered packet
            // that hasn't been sent yet.
            // This requires keeping track of unackowledgedPackets in another
            // unordered set (i.e. sent packets that haven't been acked.).
            // Since I didn't create that object, we will (for now) just
            // send the lowest packet that hasn't been acknowledged.
            if (!flow->acknowledgedPackets.count(i)) {
                lowestUnacked = i;
                break;
            }
        }
        Packet p = new Packet(std::string(lowestUnacked), flow->destination,
                              flow->source, flow->packetSize, false, false,
                              lowestUnacked);
        PacketEvent pe = new PacketEvent(p, flow->link.getId(), flow->source, time);
        host->addEvent(pe);

        UnackEvent ue = new UnackEvent(p, flow->link.getId(), flow->source, time + flow->waitTime);
        host->addEvent(ue);
    }
}
