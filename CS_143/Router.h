

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <string>
#include "Device.h"

class Router : public Device
{
public:
    // Routing table maps destination host ids to link ids
    std::map<std::string, std::string> routing_table;
    
    // Bellman-Ford
    void updateRouting(Packet);    
};


#endif /* defined(__CS_143__Router__) */
