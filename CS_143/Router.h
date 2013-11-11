

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <string>
#include "Device.h"

class Router : public Device
{
public:
    // Bellman-Ford
    void updateRouting(Packet);
    
    // create static routing table
    void createRouting(Packet);
    
    // add a link to the router
    void addLink(std::string link_id);
    
    // react to an event
    void giveEvent(std::unique_ptr<Event>);

private:
    // Routing table maps destination host ids to link ids
    std::map<std::string, std::string> routing_table;
    
    // All links connected to this router
    std::vector<std::string> links;
};


#endif /* defined(__CS_143__Router__) */
