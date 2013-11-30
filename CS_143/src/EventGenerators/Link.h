#ifndef __CS_143__Link__
#define __CS_143__Link__

#include <iostream>
#include "EventGenerator.h"
#include "../EventHandling/PacketEvent.h"
#include <list>

class Link : public EventGenerator
{

public:
    Link(float, float, float, std::string, std::string, std::string);
    
    virtual void giveEvent(std::shared_ptr<Event>) override;

    float getPropDelay();
    std::string toString();
    
    float getTotalDelay();
    
    std::string getOtherNode(std::string);
    
    void logLinkRate(float);
    
private:
    // Maximum queue_size in bits
    float buffer_size;
    
    // Delay seen by an incoming packet due to link length
    float prop_delay;
    
    // Queue size in bits at queue_time
    float queue_size;
    
    // Delay seen by an incoming packet due to the queue
    float queue_delay;
    
    // Number of bits sent per time-step
    float capacity;
    
    std::string node1;
    std::string node2;
    
    // Timestamp for which the queue is current
    float link_time;
    
    // Tuples (pkt_size, on_link_time, off_link_time)
    std::list<std::tuple<int, float, float> > packets_on_link;
};

#endif /* defined(__CS_143__Link__) */
