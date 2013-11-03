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
    // TODO: override operator> for event class
    
private:
    string const uuid;
    
    
};

class Link : public EventGenerator
{
public:
    Link::Link(float p_delay, string n1, string n2, string link_id);
    
private:
    float prop_delay;
    string node1;
    string node2;
    
};

class Device : public EventGenerator
{
    
};

class Host : public Device
{
    
};

class Router : public Device
{
    
};


#endif /* defined(__CS_143__File__) */
