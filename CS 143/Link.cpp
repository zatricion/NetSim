//
//  Link.cpp
//  CS 143
//
//  Created by Michael Lauria on 11/7/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#include "Link.h"
#include <cassert>

// Link Methods

// Constructor
Link::Link(float buf_size, float p_delay, float cap, std::string n1, std::string n2, std::string link_id)
{
    prop_delay = p_delay;
    capacity = cap;
    node1 = n1;
    node2 = n2;
    uuid = link_id;
    buffer_size = buf_size;
    queue_time = 0;
    queue_delay = 0;
    queue_size = 0;
}

Event Link::getEvent()
{
    Event nextEvent = eventHeap.top();
    eventHeap.pop();
    return nextEvent;
}

void Link::giveEvent(Event new_event)
{
    Packet new_packet = new_event.packet;
    std::string source = new_event.source;
    float now = new_event.timestamp;
    
    queue_size = std::max(0, queue_size - (now - queue_time) * capacity);
    
    
    if (queue_size < buffer_size) {
        // Add propagation and queue delay
        queue_delay = queue_size / capacity;
        float timestamp = now + prop_delay + queue_delay;
        
        // Figure out the destination
        assert (source == node1 || source == node2);
        std::string destination = (source == node1) ? node2 : node1;
        
        // Add an event to the Link priority queue
        Event packetEvent = Event(new_packet, destination, uuid, timestamp);
        eventHeap.push(packetEvent);
        queue_size++;
    }
    else
    {
        // TODO: Packets get dropped
    }
    
    queue_time = now;
}


