//
//  Packet.h
//  CS 143
//
//  Created by Michael Lauria on 11/3/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

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
    
    Packet(std::string, std::string, std::string, int, float, bool, bool, int);
};

#endif /* defined(__CS_143__Packet__) */
