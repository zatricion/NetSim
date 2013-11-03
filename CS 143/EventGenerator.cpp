//
//  CS 143
//
//  Event Generator
//  * Device
//   - Host
//   - Router
//  * Link

#include "EventGenerator.h"
#include "Event.h" // TODO: Write event class
#include <queue>

class EventGen
{
protected:
    std::priority_queue<int, std::vector<int>, std::greater<int> > eventHeap;
public:
    virtual ~EventGen();
    
    std::string getId() const;
    virtual 
    virtual void addEvent() = 0;
    virtual Event getEvent() = 0;
    
};
