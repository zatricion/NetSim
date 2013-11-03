//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router

#ifndef __CS_143__File__
#define __CS_143__File__

#include <iostream>

#include "Event.h" // TODO: Write event class
#include "Packet.h" // TODO: Write packet class
#include <queue>

class EventGenerator
{
public:
    std::string getId() const;
    
    virtual ~EventGenerator();
    
    // Call this to give the generator an event
    virtual void giveEvent(Event new_event) = 0;
    
    // Call this to get an event from the generator
    Event getEvent();
    
    // Call this to get the timestamp of the next event
    int nextTimestamp() = 0;
    
protected:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event> > eventHeap;
    // TODO: override operator> for event class
    
    
};

class Link : public EventGenerator
{
private:
    float buffer_capacity;
    float buffer_size;
    std::queue<Packet> buffer;
    
    void receivePacket(Packet new_packet);
};

class Device : public EventGenerator
{
    
};



#endif /* defined(__CS_143__File__) */
