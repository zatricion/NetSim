

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <string>
#include "Device.h"

class Router : public Device
{
public:
    // Routing table maps destination host ids to link idsomm
    std::map<std::string, std::string> routing_table;
};


#endif /* defined(__CS_143__Router__) */
