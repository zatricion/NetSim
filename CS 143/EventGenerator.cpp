//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router


#include "EventGenerator.h"


Event EventGenerator::getEvent()
{
    Event nextEvent = eventHeap.top();
    eventHeap.pop();
    return nextEvent;
}

int EventGenerator::nextTimestamp()
{
    Event nextEvent = eventHeap.top();
    return nextEvent.timestamp;
}

// Link Methods

void Link::giveEvent(Event new_event)
{
    if (new_event.type == 'packet')
    {
        receivePacket(new_event.packet);
    }
}
    

void Link::receivePacket(Packet new_packet)
{
    if (buffer_size + new_packet.size < buffer_capacity)
    {
        buffer.push(new_packet);
        buffer_size += new_packet.size;
    }
    else
    {
        signalDroppedPacket(); // TODO
    }
    
}