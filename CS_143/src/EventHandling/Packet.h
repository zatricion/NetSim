/**
 * A class to represent Packets in the network.  Only one Packet type is
 * present, but the Packet contains flags to identify whether it is a SYN,
 * FIN, ACK, or Bellman-Ford packet.  These flags are used instead of different
 * types.
 */


#ifndef __CS_143__Packet__
#define __CS_143__Packet__


#include <string>
#include <map>
#include <vector>
#include <set>
#include <memory>
#include <unordered_map>
#include "../EventGenerators/Path.h"


class Packet {

public:
    // Used for Bellman-Ford packets.
    typedef std::unordered_map<std::string, std::shared_ptr<Path> >  bf_type;
    
    // Fields
    /** An id for the packet.  Not necessarily globally unique. */
    std::string uuid;

    /** The destination. */
    std::string final_dest;

    /** The source. */
    std::string source;

    /** Size of packet. */
    int size;

    /** True if Packet is an ack. */
    bool ack;

    /** True if Packet is a SYN. */
    bool syn;

    /** True if Packet is a FIN. */
    bool fin;

    /** True if Packet is a Bellman-Ford Packet. */
    bool bf_tbl_bit;

    /** 
     * A mapping from host_id to path to that host, which is vector of node
     * ids. 
     */
    bf_type bf_table;
    
    /** Sequence number of the Packet.  Used for data packets. */
    int sequence_num;

    /** 
     * If the packet was generated from a Flow in a host, this is the ID of
     * that flow.
     */
    std::string flowID;

    /**
     * The timestamp of the Packet.  If this is a data Packet, it's the time
     * at which the Packet was sent.  If it's an ack, it's the timestamp of the
     * Packet that the ack is acking.
     */
    double timestamp;

    // Constructor.
    Packet(std::string id,
                   std::string fd,
                   std::string src,
                   int s,
                   bool a,
                   int seq,
                   std::string flow_id,
                   bool sync,
                   bool finish,
                   double ts = -1.0,
                   bool bf = false,
                   bf_type bf_table = bf_type());
    
    // Copy Constructor
    Packet(const Packet& other);

    // Accessor
    std::string toString();
};


#endif /* defined(__CS_143__Packet__) */
