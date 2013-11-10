#include "Host.h"
#include <math.h> // ceil

static const int INITIAL_WINDOW_SIZE = 0; // TODO: change this number
static const int DATA_PKT_SIZE = 1; // TODO: change this number

Host::Host(CongestionAlg congestion_algorithm , Link& host_link) : my_link(host_link)
{
    window_size = INITIAL_WINDOW_SIZE;
    num_flows = 0;
    congestion_alg = congestion_algorithm;
    my_link = host_link;
    packet_id = 0;
}

void Host::addFlow(std::string dest, float data_size, float time)
{
    float temp_num_pkts = data_size / DATA_PKT_SIZE;
    int num_packets = (int) ceil(temp_num_pkts);
    
    for (int count = 0; count < num_packets; count++){
        int index = num_flows;
        std::string pack_id = this->uuid + std::to_string(packet_id);
        Packet new_packet(pack_id, dest, this->uuid, 10, time, false, false, count+1);
        packet_queue.insert(packet_queue.begin() + index, new_packet);
        index += num_flows + 1;
        packet_id += 1;
    }
    
    num_flows += 1;
}

void Host::giveEvent(std::unique_ptr<FlowEvent> flow_event)
{
    // Add flow
    addFlow(flow_event->destination, flow_event->data_size(), flow_event->getTime());
}


void Host::giveEvent(std::unique_ptr<PacketEvent> new_event)
{
    if (new_event->type == ackPacket) {
        Packet new_packet = new_event->packet;
        std::string source = new_event->source;
        float now = new_event->eventTime();
    }

}

