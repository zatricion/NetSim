//
//  CS 143
//
//  Event Generator
//  * Device
//   - Host
//   - Router
//  * Link

#include "Event.h" // TODO: Write event class
#include <queue>

class EventGenerator
{
protected:
    std::priority_queue<Event*> eventHeap;
    std::string id;
public:
    virtual ~EventGenerator();
    
    std::string getId() const;
    // not sure these need to be virtual....
    void addEvent(Event *e);
    // return time of lowest-time event
    float getTime();
    Event* getEvent();
}
