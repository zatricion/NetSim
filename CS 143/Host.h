#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include "CongestionAlg.h"
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
