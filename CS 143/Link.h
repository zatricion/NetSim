//
//  Link.h
//  CS 143
//
//  Created by Michael Lauria on 11/7/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#ifndef __CS_143__Link__
#define __CS_143__Link__

#include <iostream>
#include "EventGenerator.h"

class Link : public EventGenerator
{
public:
    Link(float, float, float, std::string, std::string, std::string);
    void giveEvent(Event);
    std::unique_ptr<Event> getEvent();
    
private:
    // Maximum queue_size
    float buffer_size;
    
    // Delay seen by an incoming packet due to link length
    float prop_delay;
    
    // Queue size at queue_time
    float queue_size;
    
    // Delay seen by an incoming packet due to the queue
    float queue_delay;
    
    // Number of packets sent per time-step
    float capacity;
    
    std::string node1;
    std::string node2;
    
    // Timestamp for which the queue is current
    float queue_time;
};

#endif /* defined(__CS_143__Link__) */
