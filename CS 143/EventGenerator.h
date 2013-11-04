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
#include <string>
#include "Event.h" // TODO: Write event class
#include "Packet.h" // TODO: Write packet class
#include <queue>

class EventGenerator
{
public:    
    virtual ~EventGenerator();
    
    // Call this to give the generator an event
    virtual void giveEvent(Event new_event) = 0;
    
    // Call this to get an event from the generator
    virtual Event getEvent();
    
    // Call this to get the timestamp of the next event
    int nextTimestamp();
    
protected:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event> > eventHeap;
    std::string uuid;
    
    
};

class Link : public EventGenerator
{
public:
    Link(float p_delay, std::string n1, std::string n2, std::string link_id);
    void giveEvent(Event new_event);
    Event getEvent();
    
private:
    float prop_delay;
    std::string node1;
    std::string node2;
    
};

class Device : public EventGenerator
{
    
};

class Host : public Device
{
public:
    
};

class Router : public Device
{
    
};


#endif /* defined(__CS_143__File__) */
