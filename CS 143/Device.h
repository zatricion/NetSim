//
//  Device.h
//  CS 143
//
//  Created by Michael Lauria on 11/7/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#ifndef __CS_143__Device__
#define __CS_143__Device__

#include "EventGenerator.h"

class Device : public EventGenerator
{
    void giveEvent(Event);
    Event getEvent();
};

class Host : public Device
{
public:
    Host(CongestionAlg, std::string);
    int window_size;
    
    std::map
    Packet createPacket(std::string);
    void sendPacket(Packet);
    
    
};


#endif /* defined(__CS_143__Device__) */
