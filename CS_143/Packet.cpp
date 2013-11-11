#include "Packet.h"

Packet::Packet()
{
    uuid = "none";
    final_dest = "none";
    source = "none";
    size = 0;
    ack = 0;
    bell_ford = 0;
    sequence_num = -1;
}

Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               bool a,
               bool bf,
               int seq)
{
    uuid = id;
    final_dest = fd;
    source = src;    
    size = s;
    ack = a;
    bell_ford = bf;
    sequence_num = seq;
}

Packet::Packet(const Packet& other)
{
    uuid = other.uuid;
    final_dest = other.final_dest;
    source = other.source;
    size = other.size;
    ack = other.ack;
    bell_ford = other.bell_ford;
    sequence_num = other.sequence_num;
}