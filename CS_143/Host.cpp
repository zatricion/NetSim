#include "Host.h"
#include <math.h> // ceil
  
static const int INITIAL_WINDOW_SIZE = 0; // TODO: change this number
static const int DATA_PKT_SIZE = 1; // TODO: change this number

Host::Host(CongestionAlg congestion_algorithm , Link& host_link, float rt) : my_link(host_link)
{
    window_size = INITIAL_WINDOW_SIZE;
    congestion_alg = congestion_algorithm;
    my_link = host_link;
    packet_id = 0;
    unack_packets = 0;
    RTO = rt;
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

void Host::sendPacket(Packet new_pack, int time_now)
{
    // Insert packet to the eventHeap
    PacketEvent packet_event = PacketEvent(new_pack, new_pack.final_dest, new_pack.source, time_now);
    eventHeap.push(packet_event);
    
    // Insert to the eventHeap an event which is triggered once a packet has timed out
    UnackEvent unack_event = UnackEvent(new_pack, this->getId(), this->getId(), time_now + RTO);
    eventHeap.push(unack_event);
    
    unacknowledged_packets.insert(new_pack.uuid);
    unack_packets++;
}

void Host::giveEvent(std::unique_ptr<FlowEvent> flow_event)
{
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
}

void Host::giveEvent(std::unique_ptr<UnackEvent> unack_event)
{
    Packet new_pack = unack_event->packet;
    
    // Check if packet has been acknowledged, if not resend
    if (unacknowledged_packets.count(new_pack.uuid) != 0)
        sendPacket(new_pack, unack_event->eventTime());
}

void Host::giveEvent(std::unique_ptr<PacketEvent> new_event)
{
    Packet pkt = new_event->packet;
    
    if (pkt.ack)
    {
        unack_packets--;
        unacknowledged_packets.erase(pkt.uuid);

        Packet new_packet = new_event->packet;
        std::string source = new_event->source;
        float now = new_event->eventTime();
        int test;
    }
}

