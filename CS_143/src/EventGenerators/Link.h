#ifndef __CS_143__Link__
#define __CS_143__Link__

#include <iostream>
#include "EventGenerator.h"
#include "../EventHandling/PacketEvent.h"
#include <list>
#include "../IO/Plotter.h"
#include <cassert>
#include <algorithm> // std::max
#include <sstream>

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
    
    void logBufferOccupancy(float, float);
    
    void packetLoss(float);
    float updateTime;
    float tempLR;
    
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
    float link_time = 0.0;
    
    // Tuples (pkt_size, on_link_time, off_link_time)
    std::list<std::tuple<int, float, float> > packets_on_link;
    
    // Number of packets on buffer
    int buffer_occupancy;
    
    // Packets dropped in current bin of time
    int num_dropped = 0;
    
    // Last time dropped packets were logged
    float dropped_time = 0.0;
};

#endif /* defined(__CS_143__Link__) */
