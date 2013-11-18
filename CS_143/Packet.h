#ifndef __CS_143__Packet__
#define __CS_143__Packet__

#include <string>
#include <map>
#include <vector>

class Packet
{
public:
    std::string uuid;
    std::string final_dest;
    std::string source;
    
    int size;
    bool ack;
    
    bool bf_tbl_bit;
    // this is a mapping from host_id to path to that host 
    // (which is vector of node ids)
//    std::map<std::string, std::vector<std::string> > bf_table;
    
    int sequence_num;
    // If the packet was generated from a Flow in a host, this is the ID of
    // that flow.
    std::string flowID;

    // Default Constructor
    Packet();
    
    // Constructor
    Packet(std::string id,
                   std::string fd,
                   std::string src,
                   int s,
                   bool a,
                   int seq);

    // Another constructor.
    Packet(std::string id,
                   std::string fd,
                   std::string src,
                   int s,
                   bool a,
                   int seq,
                   std::string flow_id);
    
    // Copy Constructor
    Packet(const Packet& other);
    void printPacket();
};

#endif /* defined(__CS_143__Packet__) */
