#include "CongestionAlg.h"
#include "Flow.h"
#include "Host.h"

// TODO:
/*
 * CongestionAlg will be an abstract class from which all other algorithms
 * inherit.  For now, we create a "dumb" sample algorithm.
 */

// Initial call to the Congestion Algorithm.  Occurs only when the Flow object
// is first created.
void CongestionAlg::initialize(Flow* flow) {
    int windowSize = flow->windowSize;
    std::shared_ptr<Host> host = flow->host;
    
    
    // At the outset, add a number of events equal to the window size.
    for (int i = 0; i < windowSize; i++) {
        Packet p(std::to_string(i), flow->destination, flow->source,
                              flow->packetSize, false, i);
        
        // TODO the timestamps will all be the same, unless we add 
        // some value.  This should be i times the link delay, but we need
        // more changes to get that to work anyway.
        
        // Add the event.
        auto e = std::make_shared<PacketEvent>(host->my_link->getID(), flow->source, flow->timestamp + i, p);
        host->addEventToLocalQueue(e);

        // Add an event to fire when we are tired of waiting.
        auto ue = std::make_shared<UnackEvent>(p, host->getID(), flow->source, flow->timestamp + i + flow->waitTime);
        host->addEventToLocalQueue(ue);
    }
}

// Called when an event was not acknowledged.  Must update fields, resend the
// event.
// TODO need eventTime()
void CongestionAlg::handleUnackEvent(Flow* flow, Packet& unacked, float time) {
    std::shared_ptr<Host> host = flow->host;
    //Packet unacked = e->packet;
    PacketEvent e(host->my_link->getID(), flow->source, time, unacked);
    // TODO also destroy the UnackEvent;
    // TODO most algorithms will update window size, etc.
}

// Called when the flow handles an ack.
void CongestionAlg::handleAck(Flow* flow, Packet& pkt, float time) {
    std::shared_ptr<Host> host = flow->host;
    flow->acknowledgedPackets.insert(pkt.sequence_num);
    
    // After acknowledging the packet, check if there are more packets to send.
    if ((int) flow->acknowledgedPackets.size() == flow->numPackets) {
        // We are finished.  TODO destroy the flow object.
        return;
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
        // Create empty table to pass by reference
        std::map<std::string, std::vector<std::string> > table;
        
        Packet p(std::to_string(lowestUnacked), flow->destination,
                              flow->source, flow->packetSize, false,
                              lowestUnacked);
        auto pe = std::make_shared<PacketEvent>(host->my_link->getID(), flow->source, time, p);
        host->addEventToLocalQueue(pe);

        auto ue = std::make_shared<UnackEvent>(p, host->my_link->getID(), flow->source, time + flow->waitTime);
        host->addEventToLocalQueue(ue);
    }
}
