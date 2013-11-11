#include "Flow.h"
// TODO numPckts deduced from data size and packet size.
Flow::Flow(std::string id, std::string src, std::string dest,
           CongestionAlgorithm alg, int numPckts, int packetSize, Host *host) {
    source = src;
    destination = dest;
    a = alg;
    std::unordered_set<int> acknowledgedPackets;
    
    std::queue<Packet> flow;
    for (int count = 0; count < numPackets; count++) {
        std::string pack_id = this->id + std::to_string(count);
        Packet new_packet(pack_id, dest, host->getId(), packetSize, false,
                          false, count);
        flow.push(new_packet);
    }

    // TODO right here, we need to call the host's addEventToLocalQueue method
    // a bunch of times.  We also need to add the timeout events to the queue.
    // We will do both of these through a call to the congestionAlgorithm.

    // We also need to make sure that whenever we receive an
    // ack, we invoke the CongestionAlgorithm and potentially add events to
    // the queue.  The CongestionAlgorithm will have an AddEvents() method.
    // don't add them directly from the flow.

    // As a basic test, we will implement a CongestionAlgorithm that is greedy
    // and never changes window size and always tosses as many events as possible
    // onto the queue.
}

void Flow::handleUnackEvent(UnackEvent e) {
    int seqNum = e.packet.sequence_num;
    if (!unacknowledgedPackets.count(seqNum)) {
        // We didn't find the item in the set of acknowledged packets.
        // We must resend the packet.
        // TODO resend the packet; we need a hook to the Host, it would seem.
        // Or at least, access to its event queue thing.
        // Can also make this class an EventGenerator.  Wouldn't necessarily
        // recommend that.

        //Packet new_pack = unack_event->packet;

        //sendPacket(new_pack, unack_event->eventTime());

        // TODO call the CongestionAlgorithm, so it can update.

        assert false;
        return;
    }
    // else, there's nothing to do.
}

void Flow::handleAck(Packet pkt) { // TODO makes more sense if arg is an event, not packet.
    // What to do when an ack comes back.
    acknowledgedPackets.emplace(pkt.sequence_num);
    
    // TODO let the algorithm know.
    //a.update(pkt);
    // TODO have the algorithm add stuff to the host's queue.
}



// Code for populating the local event queue with send events.
// will be put into the congestionAlgorithm, sort of.

// TODO: easiest way to handle is to have 2 local event queues.
// One with only sendPacket events, one with other stuff.  Add sendPacket
// events to the one queue.  Viola!
// 
// Let's do this later.
/*
    // Create queue of packets for host to make events out of
    addFlow(flow_event->destination, flow_event->data_size);

    // Create PacketEvents based on queue of packets based on
    // number of unacknowledged packets
    int num_events = window_size - unack_packets;
    float time_now = flow_event->eventTime();
    // push PacketEvents onto eventheap
    for (int pck = 1; pck <= num_events; pck++) {
        Packet new_pack = packet_queue.pop();
        time_now +=  float(new_pack.size) / throughput;
        sendPacket(new_pack, time_now);
    }
*/
}
