#include "Router.h"

/**
 * Constructor for instance of a Router.
 *
 * @param host_list a list of hosts the router connects to.
 * @param neighboring_links a list of pointers to connected links.
 * @param router_id the router name.
 */
Router::Router(std::vector<std::string> host_list, std::vector<std::shared_ptr<Link> > neighboring_links, std::string router_id) {
    // set id
    uuid = router_id;
    
    // initialize table
    for (const auto& it : host_list) {
        auto empty_path = std::make_shared<Path>();
        addRouting(it, empty_path);
    }
    // find adjacent hosts and add them to table
    for (const auto& it : neighboring_links) {
        std::string other_node = it->getOtherNode(this->getID());
        // if the link connects to a host
        if (std::find(host_list.begin(), host_list.end(), other_node) != host_list.end()) {
            // create path of just the link that goes to the host
            auto path = std::make_shared<Path>(it->getID());
            path->updateLinkWeight(it->getID(), it->getTotalDelay(this->getID()), 0.0);
            addRouting(other_node, path);
        }
        addLink(it);
    }
    
    // broadcast the routing table to all neighboring devices
    broadcastTable(0.0);

    wait_time = 0.0;
    FILE_LOG(logDEBUG) << "waitTime=" << wait_time;
    auto b = std::make_shared<BFResendEvent>(uuid, uuid, wait_time);
    wait_time = std::min(BF_WAIT, wait_time + 0.1);
    addEventToLocalQueue(b);
}

/**
 * Broadcasts the routing table to all connected routers.
 *
 * @param timestamp the time at which the broadcast occurs.
 */
void Router::broadcastTable(double timestamp) {
    // update BF table
    for (const auto& it : links) {
        double delay = it.second->getTotalDelay(this->getID());
        for (auto& bf : routing_table) {
            bf.second->updateLinkWeight(it.first, delay, timestamp);
        }
    }
    // send BF table
    for (const auto& it : links) {
        // get connected router
        std::string other_node = it.second->getOtherNode(this->getID());
        auto pkt = std::make_shared<Packet>("bf_pkt_" + this->getID(),
                                            other_node, this->getID(),
                                            BF_PKT_SIZE,
                                            false, // ack
                                            0, // seq num
                                            "NONE", // flowID
                                            false, // sync
                                            false, // finish
                                            timestamp,
                                            true,
                                            routing_table);
        auto bf_event = std::make_shared<PacketEvent>(it.first, this->getID(), timestamp, pkt);
        eventHeap.push(bf_event);
    }
}

/**
 * Adds a link to the router's link vector.
 *
 * @param link a pointer to the link to add.
 */
void Router::addLink(std::shared_ptr<Link> link) {
    links[link->getID()] = link;
}

/**
 * Adds a host_id -> (next_link_id, dist, path) tuple indicating which link to route to 
 * given a host, along with bellman-ford info
 *
 * @param host_id the name of the host to which the router is routing packets.
 * @param path a pointer to the path it will use to route packets.
 */
void Router::addRouting(std::string host_id, std::shared_ptr<Path> path) {
    routing_table[host_id] = path;
}

/**
 * Returns the proper link to route to given a host
 *
 * @param targ_host the name of the host to which the router is routing packets.
 * @return the name of the next link in the path.
 */
std::string Router::getRouting(std::string targ_host) {
    return routing_table[targ_host]->getNextLink();
}

/**
 * Updates the routing table of a router when it receives a bf_packet.
 *
 * @param bf_table the routing table received.
 * @param link_id the name of the link from which the packet was received.
 * @param now the timestamp of the packet event.
 */
void Router::updateRouting(Packet::bf_type bf_table,
                           std::string link_id,
                           double now) {
    // update the delays of the links in our path before updating paths
    updateTableWeights(bf_table);
    
    // update neighboring links
    for (const auto& li : links) {
        double delay = li.second->getTotalDelay(this->getID());
        for (auto& bf : routing_table) {
            bf.second->updateLinkWeight(li.first, delay, now);
        }
    }
    
    // Delay of link to other router
    double link_delay = links[link_id]->getTotalDelay(this->getID());
    
    for (const auto& it : bf_table) {
        std::string host_id = it.first;
        
        Path other_path = *it.second;
        
        // Delay along the other path
        double other_delay = other_path.getTotalDelay();
        
        // Delay along the current path
        double curr_delay = routing_table[host_id]->getTotalDelay();
        
        if (link_delay + other_delay < curr_delay) {
            // check to see if current router is already in other router path
            // to avoid cycles
            if (!other_path.hasCycle(link_id)) {
                // replace old path with new path
                other_path.addLink(link_id, link_delay, now);
                auto new_path = std::make_shared<Path>(other_path);
                addRouting(host_id, new_path);
            }
        }
    }
}

/**
 * Give an event to the router.  This method assumes it to be a packet event.
 *
 * @param e the event to be processed
 */
void Router::giveEvent(std::shared_ptr<Event> e) {
    FILE_LOG(logDEBUG) << "TYPE OF E:" << e->getType();
    // Get PacketEvent
    if (e->getType() == "BF_RESEND_EVENT") {
        double now = e->eventTime();
        FILE_LOG(logDEBUG) << "Scheduling BF Resend";
        broadcastTable(now);
        auto b = std::make_shared<BFResendEvent>(uuid, uuid, now + wait_time);
        wait_time = std::min(5.0, wait_time + 0.1);
        addEventToLocalQueue(b);
        return;
    }

    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));
    
    // strip necessary info out of packet event
    std::shared_ptr<Packet> pkt = packet_event.packet;
    
    if (pkt->bf_tbl_bit) {
        // Need to update routing.
        updateRouting(pkt->bf_table, packet_event.source, packet_event.eventTime());
    }
    else {
        // Routing actual data
        std::string dest = getRouting(pkt->final_dest);
        
        if (dest != "NONE") {
            // make new event
            auto new_event = std::make_shared<PacketEvent>(dest, this->getID(), packet_event.eventTime(), pkt);
            // put on event heap
            eventHeap.push(new_event);
        }
    }
}

/**
 * Updates the link delays in a routing table given a new table.
 * The link delays only change if they see a more current delay in the new table.
 *
 * @param other_table the table with new information about link delays
 */
void Router::updateTableWeights(Packet::bf_type other_table) {
    for (const auto& it : other_table) {
        std::string host_id = it.first;
        routing_table[host_id]->updateAll(*it.second);
    }
}

/**
 * Outputs a text representation of a router.
 */
std::string Router::toString() {
    std::stringstream str;
    str << "Router: " + this->getID() + "\n";
    str << "Size of table: " << std::to_string(routing_table.size()) + "\n";
    
    for (const auto& it : routing_table) {
        std::string host_id = it.first;
        Path other_path = *it.second;
        double curr_delay = other_path.getTotalDelay();
        std::string link_id = other_path.getNextLink();
        std::string path_str = other_path.to_string();
        
        std::string host_row_str =
            "Host: " + host_id + "\nLink_ID: " + link_id + "\nLink_delay: " + std::to_string(curr_delay) + "\nOther Path: " + path_str + "\n";
        
        str << host_row_str + "\n";
    }
    return str.str();
}
 
