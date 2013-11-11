#include "Host.h"
#include <math.h> // ceil

static const int INITIAL_WINDOW_SIZE = 0; // TODO: change this number
static const int DATA_PKT_SIZE = 1; // TODO: change this number

Host::Host(CongestionAlg congestion_algorithm , Link& host_link) : my_link(host_link)
{
    window_size = INITIAL_WINDOW_SIZE;
    congestion_alg = congestion_algorithm;
    my_link = host_link;
    packet_id = 0;
    unack_packets = 0;
}

void Host::addFlow(std::string dest, float data_size)
{
    float temp_num_pkts = data_size / DATA_PKT_SIZE;
    int num_packets = (int) ceil(temp_num_pkts);
    
    std::queue<Packet> flow;
    
    for (int count = 0; count < num_packets; count++){
        std::string pack_id = this->uuid + std::to_string(packet_id);
        Packet new_packet(pack_id, dest, this->uuid, DATA_PKT_SIZE, false, false, count+1);
        flow.push(new_packet);
        packet_id++;
    }
    
    packet_queue.addQueue(flow);
}

void Host::giveEvent(std::unique_ptr<FlowEvent> flow_event)
{
    // Create queue of packets for host to make events out of
    addFlow(flow_event->destination, flow_event->data_size());
    
    // Create PacketEvents based on queue of packets based on
    // number of unacknowledged packets
    int num_events = window_size - unack_packets;
    time_now = flow_event->eventTime();
    // push PacketEvents onto eventheap
    for (int pck = 1; pck <= num_events; pck++) {
        Packet new_pack = packet_queue.pop();
        time_now +=  float(new_pack.size) / throughput
        PacketEvent packetEvent = Event(&new_pack, new_pack->final_dest, new_pack->source, time_now);
        eventHeap.push(packetEvent);
        unack_packets++;
    }
}

PacketEvent::PacketEvent(Packet& pkt,
                         std::unique_ptr<EventGenerator> dest,
                         std::unique_ptr<EventGenerator> src,
                         float ts) : packet(pkt)
{
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
    packet = pkt;
}

void Host::giveEvent(std::unique_ptr<PacketEvent> new_event)
{
    if (new_event->type == ackPacket) {
        Packet new_packet = new_event->packet;
        std::string source = new_event->source;
        float now = new_event->eventTime();
    }

}

