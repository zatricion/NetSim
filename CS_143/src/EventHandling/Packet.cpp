#include "Packet.h"
#include <iostream>
#include <string>
#include <sstream>

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
    syn = 0;
    fin = 0;
    std::set<int> ackSet;
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
    syn = 0;
    fin = 0;
    std::set<int> ackSet;
}

// Constructor for when a packet is generated from a host.  We need the flowID
// of the flow from which the packet was derived.
Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               bool a,
               int seq,
               std::string flow_id,
               bool sync,
               bool finish)
{
    uuid = id;
    final_dest = fd;
    source = src;
    size = s;
    ack = a;
    sequence_num = seq;
    flowID = flow_id;
    syn = sync;
    fin = finish;
    std::set<int> ackSet;
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
    syn = other.syn;
    fin = other.fin;
    ackSet = other.ackSet;
}

std::string Packet::toString() {
    std::stringstream fmt;
    fmt << "{PACKET: uuid=" << uuid << ", dest=" << final_dest <<
           ", source=" << source << ", size=" << size << ", ack=" << ack <<
           ", sequence_num=" << sequence_num << ", flowID=" << flowID << ", syn=" << syn << ", fin=" << fin << "}.";
    return fmt.str();
}
