//
//  Packet.cpp
//  CS 143
//
//  Created by Michael Lauria on 11/3/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#include "Packet.h"

Packet::Packet(std::string id,
               std::string fd,
               std::string src,
               int s,
               float tc,
               bool a,
               bool bf,
               int seq) {
    
    uuid = id;
    final_dest = fd;
    source = src;    
    size = s;
    time_created = tc;
    ack = a;
    bell_ford = bf;
    sequence_num = seq;
}