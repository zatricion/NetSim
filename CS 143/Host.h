//
//  Host.h
//  CS 143
//
//  Created by Michael Lauria on 11/7/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include "Device.h"

class Host : public Device
{
public:
    Host(CongestionAlg, std::string);
    int window_size;
    
    Packet createPacket(std::string);
    void sendPacket(Packet);
};



#endif /* defined(__CS_143__Host__) */
