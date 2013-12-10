/**
 * Implementation of Network Routers.
 */


#ifndef __CS_143__Router__
#define __CS_143__Router__


#include <unordered_map>
#include <string>
#include <algorithm>
#include <limits>
#include <sstream>
#include <fstream>
#include "Host.h"
#include "Path.h"
#include "../EventHandling/Packet.h"
#include "../EventHandling/PacketEvent.h"
#include "../EventHandling/BFResendEvent.h"


static const int BF_PKT_SIZE = 1024 * 8;
static const double BF_WAIT = 5.0;


class Router : public EventGenerator {
public:
    // Constructor
    Router(std::vector<std::string> host_list, 
           std::vector<std::shared_ptr<Link> > neighboring_links,
           std::string router_id);
    
    // Methods
    void updateRouting(Packet::bf_type, std::string, double);
    void broadcastTable(double timestamp);
    void addRouting(std::string targ_host, std::shared_ptr<Path> path);
    void addLink(std::shared_ptr<Link> link);
    void giveEvent(std::shared_ptr<Event>);
    void updateTableWeights(Packet::bf_type);
    std::string getRouting(std::string targ_host);
    
    // Accessors
    std::string toString();
    

private:
    // Fields
    /** A representation of the routing table. */
    Packet::bf_type routing_table;
    
    /** A map of all links connected to this router. */
    std::unordered_map<std::string, std::shared_ptr<Link> > links;
    
    /**
     * The time to wait between B-F table broadcasts.  Used alongside 
     * BFRoutingUpdateEvent.
     */
    double wait_time = 0.0;
};


#endif /* defined(__CS_143__Router__) */
