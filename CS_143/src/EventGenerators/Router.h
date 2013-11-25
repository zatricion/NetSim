

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <unordered_map>
#include <string>
#include <algorithm>
#include <limits>
#include "../EventHandling/Packet.h"
#include "../EventHandling/PacketEvent.h"
#include "Host.h"

static const int BF_PKT_SIZE = 1024 * 8;

class Router : public EventGenerator
{
public:
    // Constructor
    Router(std::vector<std::string> host_list, std::vector<std::shared_ptr<Link> > neighboring_links);
    
    // Bellman-Ford
    void updateRouting(Packet::bf_type, std::string);
    
    // create static routing table
    void addRouting(std::string targ_host, std::string next_link_id, float dist, std::vector<std::string> path);
    
    // add a link to the router
    void addLink(std::shared_ptr<Link> link);

    // get proper routing given host id
    std::string getRouting(std::string targ_host);

    // react to a packet event
    void giveEvent(std::shared_ptr<Event>);

private:
    // Routing table maps destination host ids to (id of next link, total distance of path, path)
    Packet::bf_type routing_table;
    
    // All links connected to this router
    std::unordered_map<std::string, std::shared_ptr<Link> > links;
};


#endif /* defined(__CS_143__Router__) */
