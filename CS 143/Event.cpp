//
//  Event.cpp
//  CS 143
//
//  Created by Michael Lauria on 11/3/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//
#include <Event.hh>

#include "Event.h"

// Constructor

Event::Event(Packet pkt, std::string dest, std::string src, float ts)
{
    packet = pkt;
    destination = dest;
    source = src;
    timestamp = ts;
bool Event::operator<(const Event &rhs) {
    return this->time < rhs.time;
}


// Override operator to allow eventHeap to order by timestamp
bool Event::operator>(const Event& other) const
{
    return timestamp > other.timestamp;
float Event::getTime() {
    return this->time;
}

