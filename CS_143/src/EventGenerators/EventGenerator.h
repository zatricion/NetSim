/**
 * Superclass for Links, Hosts, Routers, and FlowGenerators.  EventGenerators 
 * each have their own eventHeaps, which are ordered lists of events.  The
 * EventHandler will determine the Event in the eventHeaps of the set of
 * EventGenerators with the minimum time, and handle the Event.
 */


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
    /** The unique id of the EventGenerator. */
    std::string uuid;
    /**
     * The eventHeap, which stores a list of events that the the 
     * EventGenerator will give to the EventHandler.
     */
    std::priority_queue<std::shared_ptr<Event>,
                        std::vector<std::shared_ptr<Event> >,
                        std::greater<std::shared_ptr<Event>> > eventHeap;

    // Constructors:

    // Accessors:
    std::string getID() const;

    // Methods:
    /**
     * Hands an Event to the EventGenerator for processing.
     *
     * @param e the Event to be processed.
     */
    virtual void giveEvent(std::shared_ptr<Event> e) = 0;

    bool hasEvents();
    std::shared_ptr<Event> getEvent();
    double getNextTime();
    void addEventToLocalQueue(std::shared_ptr<Event> e);
};


#endif /* defined(__CS_143__EventGenerator__) */
