//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router

#ifndef __CS_143__EventGenerator__
#define __CS_143__EventGenerator__

#include "deref_comp.h"
#include <string>
#include "Event.h" 
#include <queue>
#include <map>
#include <memory>

class EventGenerator
{
public:        
    std::string getID() const;
    
    // Call this to give the generator an event
    virtual void giveEvent(std::shared_ptr<Event>) = 0;
    
    // Call this to get an event from the generator
    std::shared_ptr<Event> getEvent();
    
    // Call this to get the timestamp of the next event
    float getNextTime();

    bool hasEvents();
    
    // Add event to local priority queue.
    void addEventToLocalQueue(std::shared_ptr<Event> e);
    
    long heapSize();
    
    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event> >, std::greater<std::shared_ptr<Event>> > eventHeap;
    std::string uuid;
    void printHeapSize();
};



#endif /* defined(__CS_143__EventGenerator__) */
