//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router


#include "EventGenerator.h"
#include "Event.h"
#include <cassert>

int EventGenerator::nextTimestamp()
{
    Event nextEvent = eventHeap.top();
    return nextEvent.timestamp;
}

// Link Methods

// Constructor
Link::Link(float p_delay, string n1, string n2, string link_id)
{
    prop_delay = p_delay;
    node1 = n1;
    node2 = n2;
    uuid = link_id;
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
    string source = new_event.source;
    
    // Add propagation delay
    timestamp = new_event.timestamp + prop_delay;

    assert (source == node1 || source == node2);
    string destination = (source == node1) ? node2 : node1;
    
    Event packetEvent = new Event(new_packet, destination, uuid, timestamp);
    eventHeap.push(packetEvent);
}

// Device Methods
