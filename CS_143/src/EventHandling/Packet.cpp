#include "Packet.h"
#include <iostream>
#include <string>
#include <sstream>


/**
 * Create a Packet object.
 * 
 * @param id an ID representing the Packet.
 * @param fd the destination of the Packet.
 * @param src the source of the Packet.
 * @param s the size of the Packet.
 * @param a true if the Packet is an ack.
 * @param seq the sequence number of the Packet.  In the case of a data Packet,
 * this will usually (but not always) be an int version of the id of the Packet.
 * In the case of an ack, this will usually be the next Packet expected.
 * @param flow_id the id of the Flow from which the Packet was sent.
 * @param sync true if the Packet is a SYN
 * @param finish true if the Packet is a FIN
 * @param ts a timestamp associated with the Packet.  For data packets, this
 * will be the time at which the Packet is sent, but for acks, this will be
 * the time at which the ack's data Packet was sent.
 * @param bf true if the Packet is a bellman ford packet (Router use only).
 * @param bf_t a representation of the routing table, if bf=true.
 * Empty otherwise.
 */
Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               bool a,
               int seq,
               std::string flow_id,
               bool sync,
               bool finish,
               double ts,
               bool bf,
               bf_type bf_t) {
    uuid = id;
    final_dest = fd;
    source = src;
    size = s;
    ack = a;
    sequence_num = seq;
    flowID = flow_id;
    syn = sync;
    fin = finish;
    bf_tbl_bit = bf;
    bf_table = bf_t;
    timestamp = ts;
}


/**
 * Copy constructor.
 *
 * @param other Packet to copy.
 */
Packet::Packet(const Packet& other) {
    uuid = other.uuid;
    final_dest = other.final_dest;
    source = other.source;
    size = other.size;
    ack = other.ack;
    sequence_num = other.sequence_num;
    flowID = other.flowID;
    syn = other.syn;
    fin = other.fin;
    bf_tbl_bit = other.bf_tbl_bit;
    bf_table = other.bf_table;
}


/**
 * String representation of the Packet.
 *
 * @return the Packet as a string.
 */
std::string Packet::toString() {
    std::stringstream fmt;
    fmt << "{PACKET: uuid=" << uuid << ", dest=" << final_dest <<
           ", source=" << source << ", size=" << size << ", ack=" << ack <<
           ", sequence_num=" << sequence_num << ", flowID=" << flowID << ", syn=" << syn << ", fin=" << fin << ", timestamp=" << timestamp << "}.";
    return fmt.str();
}
