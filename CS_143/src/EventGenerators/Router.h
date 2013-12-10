

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <unordered_map>
#include <string>
#include <algorithm>
#include <limits>
#include "../EventHandling/Packet.h"
#include "../EventHandling/PacketEvent.h"
#include "../EventHandling/BFResendEvent.h"
#include <sstream>
#include <fstream>
#include "Host.h"
#include "Path.h"

static const int BF_PKT_SIZE = 1024 * 8;
static const double BF_WAIT = 5.0;

class Router : public EventGenerator
{
public:
    // Constructor
    Router(std::vector<std::string> host_list, std::vector<std::shared_ptr<Link> > neighboring_links, std::string router_id);
    
    // Bellman-Ford
    void updateRouting(Packet::bf_type, std::string, double);
    
    // Broadcast routing table
    void broadcastTable(double timestamp);
    
    // add route to routing table
    void addRouting(std::string targ_host, std::shared_ptr<Path> path);
    
    // add a link to the router
    void addLink(std::shared_ptr<Link> link);

    // get proper routing given host id
    std::string getRouting(std::string targ_host);

    // react to a packet event
    void giveEvent(std::shared_ptr<Event>);
    
    std::string toString();
    
    // update the link weights in a routing table
    void updateTableWeights(Packet::bf_type);

private:
    // Routing table maps destination host ids to (id of next link, total distance of path, path)
    Packet::bf_type routing_table;
    
    // All links connected to this router
    std::unordered_map<std::string, std::shared_ptr<Link> > links;
    
    // Time to wait between bf packet broadcasts
    double wait_time = 0.0;
};


#endif /* defined(__CS_143__Router__) */
