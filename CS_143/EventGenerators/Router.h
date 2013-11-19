

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <unordered_map>
#include <string>
#include "../Packet.h"
#include "../PacketEvent.h"

class Router : public EventGenerator
{
public:
    // Bellman-Ford
    //void updateRouting(Packet);
    
    // create static routing table
    void addRouting(std::string targ_host, std::string targ_link);
    
    // add a link to the router
    void addLink(std::string link_id);

    // get proper routing given host id
    std::string getRouting(std::string targ_host);

    // react to a packet event
    void giveEvent(std::shared_ptr<Event>);

private:
    // Routing table maps destination host ids to link ids
    std::unordered_map<std::string, std::string> routing_table;
    
    // All links connected to this router
    std::vector<std::string> links;
};


#endif /* defined(__CS_143__Router__) */
