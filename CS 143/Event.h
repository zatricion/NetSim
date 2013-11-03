//
//  Header.h
//  CS 143
//
//  Created by Michael Lauria on 11/3/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#ifndef CS_143_Header_h
#define CS_143_Header_h

#include "Packet.h"
#include <string>

class Event
{
public:
    Event::Event(Packet pkt, string dest, string src, int ts)
    
    Packet packet;
    int timestamp;
    string destination;
    string source;

    bool operator>(const Event& other) const;
};

#endif
