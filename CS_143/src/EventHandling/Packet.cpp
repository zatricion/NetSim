#include "Packet.h"
#include <iostream>
#include <string>
#include <sstream>

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

// Copy Constructor
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

std::string Packet::toString() {
    std::stringstream fmt;
    fmt << "{PACKET: uuid=" << uuid << ", dest=" << final_dest <<
           ", source=" << source << ", size=" << size << ", ack=" << ack <<
           ", sequence_num=" << sequence_num << ", flowID=" << flowID << "}.";
    return fmt.str();
}
