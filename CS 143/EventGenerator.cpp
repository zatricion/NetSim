//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router


#include "EventGenerator.h"
#include "Event.h" // TODO: Write event class
#include "Packet.h" // TODO: Write packet class
#include <queue>

class EventGenerator
{
protected:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event> > eventHeap;
    // TODO: override operator> for event class
    
public:
    virtual ~EventGenerator();
    
    std::string getId() const;

    virtual void addEvent() = 0;
    virtual Event getEvent() = 0;
    
};

class Link : public EventGenerator
{
private:
    std::queue<Packet>
    
};

class Device : public EventGenerator
{
public:
    void addEvent() {
        
    }
    
    Event getEvent() {
        
    }
    
};

