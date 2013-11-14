#include "Packet.h"
#include <iostream>

// Default constructor.
Packet::Packet()
{
    uuid = "none";
    final_dest = "none";
    source = "none";
    size = 0;
    ack  = 0;
    sequence_num = -1;
    flowID = "NONE";
}

// Constructor for most cases.
Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               bool a,
               int seq)
{
    uuid = id;
    final_dest = fd;
    source = src;    
    size = s;
    ack = a;
    sequence_num = seq;
    flowID = "NONE";
}

// Constructor for when a packet is generated from a host.  We need the flowID
// of the flow from which the packet was derived.
Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               bool a,
               int seq,
               std::string flow_id)
{
    uuid = id;
    final_dest = fd;
    source = src;
    size = s;
    ack = a;
    sequence_num = seq;
    flowID = flow_id;
}

Packet::Packet(const Packet& other)
{
    uuid = other.uuid;
    final_dest = other.final_dest;
    source = other.source;
    size = other.size;
    ack = other.ack;
    sequence_num = other.sequence_num;
    flowID = other.flowID;
}

void Packet::printPacket() {
    std::cout << "PACKET" << std::endl;
    std::cout << "uuid:" << uuid << std::endl;
    std::cout << "dest:" << final_dest << std::endl;
    std::cout << "source:" << source << std::endl;
    std::cout << "size:" << std::to_string(size) << std::endl;
    std::cout << "ack:" << std::to_string(ack) << std::endl;
    std::cout << "seq_num:" << std::to_string(sequence_num) << std::endl;
    std::cout << "flowID:" << flowID << std::endl;
}
