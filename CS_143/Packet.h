#ifndef __CS_143__Packet__
#define __CS_143__Packet__

#include <string>

class Packet
{
public:
    std::string uuid;
    std::string final_dest;
    std::string source;
    
    int size;
    
    // When the packet was sent
    float time_created;
    
    bool ack;
    bool bell_ford;
    
    int sequence_num;
    
    // Default Constructor
    Packet();
    
    // Constructor
    Packet(std::string, std::string, std::string, int, bool, bool, int);
    
    // Copy Constructor
    Packet(const Packet& other);
};

#endif /* defined(__CS_143__Packet__) */
