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

#include <string>
#include "Event.h" 
#include "Packet.h"
#include "CongestionAlg.h"
#include <queue>
#include <map>

class EventGenerator
{
public:    
    virtual ~EventGenerator();
    
    std::string getID() const;
    
    // Call this to give the generator an event
    virtual void giveEvent(std::unique_ptr<Event> new_event) = 0;
    
    // Call this to get an event from the generator
    virtual std::unique_ptr<Event> getEvent();
    
    // Call this to get the timestamp of the next event
    float getNextTime();
    
protected:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event> > eventHeap;
    std::string uuid;
    
    
};


#endif /* defined(__CS_143__EventGenerator__) */
