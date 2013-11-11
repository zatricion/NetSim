#ifndef __CS_143__Packet__
#define __CS_143__Packet__

#include <string>

class Packet {
public:
    std::string uuid;
    std::string final_dest;
    std::string source;
    
    int size;
    bool ack;
    
    bool bf_req_bit;
    bool bf_tbl_bit;
    // this is a mapping from host_id to path to that host 
    // (which is vector of node ids)
    std::map<std::string, std::vector<std::string> > bf_table;
    
    int sequence_num;

    // Default Constructor
    Packet();
    
    // Constructor
    Packet(std::string, std::string, std::string, int, bool, bool, bool,
	   std::map<std::string, std::vector<std::string> >, int);
    
    // Copy Constructor
    Packet(const Packet& other);
};

#endif /* defined(__CS_143__Packet__) */
