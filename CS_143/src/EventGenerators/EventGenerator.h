#ifndef __CS_143__EventGenerator__
#define __CS_143__EventGenerator__


#include "../Tools/deref_comp.h"
#include <string>
#include "../EventHandling/Event.h" 
#include <queue>
#include <map>
#include <memory>


class EventGenerator {

public:        
    // Fields:
    std::string uuid;
    std::priority_queue<std::shared_ptr<Event>,
                        std::vector<std::shared_ptr<Event> >,
                        std::greater<std::shared_ptr<Event>> > eventHeap;

    // Constructors:

    // Accessors:
    std::string getID() const;

    // Methods:
    bool hasEvents();
    virtual void giveEvent(std::shared_ptr<Event>) = 0;
    std::shared_ptr<Event> getEvent();
    double getNextTime();
    void addEventToLocalQueue(std::shared_ptr<Event> e);
};


#endif /* defined(__CS_143__EventGenerator__) */
