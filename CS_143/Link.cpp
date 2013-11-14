#include "Link.h"
#include <cassert>
#include <iostream>
#include <algorithm> // std::max
#include "make_unique.h"


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

void Link::giveEvent(std::unique_ptr<PacketEvent> e)
{
    Packet new_packet = e->packet;
    std::string source = e->source;
    float now = e->eventTime();
    
    // Queue size in bits
    queue_size = std::max<float>(0, queue_size - (now - queue_time) * capacity);
    
    if (queue_size + new_packet.size < buffer_size)
    {
        
        queue_delay = (queue_size + new_packet.size) / capacity;
        
        // Add propagation and queue delay to current time to get event time
        float timestamp = now + prop_delay + queue_delay;
        
        // Figure out the destination
        assert (source == node1 || source == node2);
        std::string destination = (source == node1) ? node2 : node1;
        
        // Add an event to the Link priority queue
        PacketEvent packetEvent = PacketEvent(destination, uuid, timestamp, new_packet);
        eventHeap.push(&packetEvent);
        
        // Update queue size
        queue_size += new_packet.size;
    }
    
    else
    {
        // TODO: Packets get dropped
    }
    
    queue_time = now;
}


