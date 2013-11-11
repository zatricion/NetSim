//
//  Event.h
//  CS 143
//
//  Created by Michael Lauria on 11/10/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#ifndef __CS_143__Event__
#define __CS_143__Event__

#include <iostream>
#include "Packet.h"
#include <string>

class Event
{
public:
    Event(std::string dest, std::string src, float ts);
    
    float eventTime() const;
    
    void handleEvent(std::unique_ptr<Event> self);
    
    std::string destination;
    std::string source;
    
    bool operator>(const Event& other) const;
    
protected:
    float timestamp;
};

#endif /* defined(__CS_143__Event__) */
