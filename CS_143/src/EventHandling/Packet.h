#ifndef __CS_143__Packet__
#define __CS_143__Packet__

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

class Packet
{
public:
    typedef std::unordered_map<std::string, std::tuple<std::string, float, std::vector<std::string> > > bf_type;
    
    std::string uuid;
    std::string final_dest;
    std::string source;
    
    int size;
    bool ack;
    
    bool bf_tbl_bit;
    
    // this is a mapping from host_id to path to that host 
    // (which is vector of node ids)
    bf_type bf_table;
    
    int sequence_num;
    // If the packet was generated from a Flow in a host, this is the ID of
    // that flow.
    std::string flowID;

    // Another constructor.
    Packet(std::string id,
                   std::string fd,
                   std::string src,
                   int s,
                   bool a,
                   int seq,
                   std::string flow_id,
                   bool bf = false,
                   bf_type bf_table = bf_type());
    
    // Copy Constructor
    Packet(const Packet& other);
    std::string toString();
};

#endif /* defined(__CS_143__Packet__) */
