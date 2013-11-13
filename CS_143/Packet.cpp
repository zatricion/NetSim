#include "Packet.h"

// Default constructor.
Packet::Packet()
{
    uuid = "none";
    final_dest = "none";
    source = "none";
    size = 0;
    ack  = 0;
    bf_req_bit = 0;
    bf_tbl_bit = 0;
    std::map<std::string, std::vector<std::string> > empty_table;
    bf_table = empty_table;
    sequence_num = -1;
}

// Constructor for most cases.
Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               bool a,
               bool bf_request_bit,
               bool bf_table_bit,
	       std::map<std::string, std::vector<std::string> > &table,
               int seq)
{
    uuid = id;
    final_dest = fd;
    source = src;    
    size = s;
    ack = a;
    bf_req_bit = bf_request_bit;
    bf_tbl_bit = bf_table_bit;
    bf_table = table;
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
               bool bf_request_bit,
               bool bf_table_bit,
               std::map<std::string, std::vector<std::string> > &table,
               int seq,
               std::string flow_id)
{
    uuid = id;
    final_dest = fd;
    source = src;
    size = s;
    ack = a;
    bf_req_bit = bf_request_bit;
    bf_tbl_bit = bf_table_bit;
    bf_table = table;
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
    bf_req_bit = other.bf_req_bit;
    bf_tbl_bit = other.bf_tbl_bit;
    bf_table = other.bf_table;
    sequence_num = other.sequence_num;
}
