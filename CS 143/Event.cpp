//
//  Event.cpp
//  CS 143
//
//  Created by Michael Lauria on 11/3/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#include "Event.h"

// Constructor

Event::Event(Packet pkt, string dest, string src, int ts)
{
    packet = pkt;
    destination = dest;
    source = src;
    timestamp = ts;
}


// Override operator to allow eventHeap to order by timestamp
bool Event::operator>(const Event& other) const
{
    return timestamp > other.timestamp;
}

