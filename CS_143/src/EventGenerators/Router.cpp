#include "Router.h"
#include <fstream>

// Constructor
Router::Router(std::vector<std::string> host_list, std::vector<std::shared_ptr<Link> > neighboring_links, std::string router_id, std::vector<std::shared_ptr<Link> > d_links) {
    // set id
    uuid = router_id;
    debug_links = d_links;
    
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
            path->updateLinkWeight(it->getID(), it->getTotalDelay(this->getID()));
            addRouting(other_node, path);
        }
        addLink(it);
    }
    // add bf packetEvents to eventHeap
    broadcastTable(0.0);

    wait_time = 0.0;
    FILE_LOG(logDEBUG) << "waitTime=" << wait_time;
    auto b = std::make_shared<BFResendEvent>(uuid, uuid, wait_time);
    wait_time = std::min(5.0, wait_time + 0.1); // TODO 5.0 magic #
    addEventToLocalQueue(b);
}

// broadcast BF table to all connected routers
void Router::broadcastTable(float timestamp) {
    // update BF table
    for (const auto& it : links) {
        float delay = it.second->getTotalDelay(this->getID());
        for (auto& bf : routing_table) {
            bf.second->updateLinkWeight(it.first, delay);
        }
    }
    // send BF table
    for (const auto& it : links) {
        // get connected router
//        if (this->getID() == "router1" && it.first == "link2") {
//            printf("Link 0 delay = %f\n", it.second->getTotalDelay());
//        }
        std::string other_node = it.second->getOtherNode(this->getID());
        auto pkt = std::make_shared<Packet>("bf_pkt_" + this->getID(),
                                            other_node, this->getID(),
                                            BF_PKT_SIZE,
                                            false,
                                            0,
                                            "NONE",
                                            false,
                                            false,
                                            true,
                                            routing_table);
        auto bf_event = std::make_shared<PacketEvent>(it.first, this->getID(), timestamp, pkt);
        eventHeap.push(bf_event);
    }
}

// Add a link id to the router's link vector
void Router::addLink(std::shared_ptr<Link> link) {
    links[link->getID()] = link;
}

// Add a host_id -> (next_link_id, dist, path) tuple indicating which link to route to given a host, along with bellman-ford info
void Router::addRouting(std::string host_id, std::shared_ptr<Path> path) {
    routing_table[host_id] = path;
}

// Return the proper link to route to given a host
std::string Router::getRouting(std::string targ_host) {
    // should probably do some error checking
    return routing_table[targ_host]->getNextLink();
}

void Router::updateRouting(Packet::bf_type bf_table, std::string link_id, std::string router) { // router is for debugging
    
    // update the delays of the links in our path before updating paths
    updateTableWeights(bf_table);
    
    // update neighboring links
    for (const auto& li : links) {
        float delay = li.second->getTotalDelay(this->getID());
        for (auto& bf : routing_table) {
            bf.second->updateLinkWeight(li.first, delay);
        }
    }
    
    // Delay of link to other router
    float link_delay = links[link_id]->getTotalDelay(this->getID());
    
    for (const auto& it : bf_table) {
        std::string host_id = it.first;
        
        Path other_path = *it.second;
        
        // Delay along the other path
        float other_delay = other_path.getTotalDelay();
        
        // Delay along the current path
        float curr_delay = routing_table[host_id]->getTotalDelay();
        
        if (link_delay + other_delay < curr_delay) {
            // check to see if current router is already in other router path
            // to avoid cycles
            if (!other_path.hasCycle(link_id)) {
                // replace old path with new path
                other_path.addLink(link_id, link_delay);
                auto new_path = std::make_shared<Path>(other_path);
                addRouting(host_id, new_path);
            }
        }
    }
    
    printRouting(routing_table, this->getID());
//    if (this->getID() == "router1" || this->getID() == "router4") {
//        printRouting(bf_table, router);
//        printRouting(routing_table, this->getID() + "_after");
//    }
}

// Deal with PacketEvents
void Router::giveEvent(std::shared_ptr<Event> e) {
    FILE_LOG(logDEBUG) << "TYPE OF E:" << e->getType();
    // Get PacketEvent
    if (e->getType() == "BF_RESEND_EVENT") {
        float now = e->eventTime();
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
        updateRouting(pkt->bf_table, packet_event.source, pkt->source);
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

// update the link weights in a routing table
void Router::updateTableWeights(Packet::bf_type other_table) {
    for (const auto& it : other_table) {
        std::string host_id = it.first;
        routing_table[host_id]->updateAll(*it.second);
    }
}

void Router::printRouting(Packet::bf_type r_table, std::string router) {
    std::ofstream myFile;
    myFile.open("routing_table_test.txt", std::ios::out | std::ios::app);
    std::string r_str = "\nRouter: " + router + "\n";
    myFile << r_str;
    myFile << "Size of table: " + std::to_string(r_table.size()) + "\n";
    for (const auto& q : debug_links) {
        myFile << q->getID().c_str() << " delay: " << std::to_string(q->getTotalDelay(uuid)) + "\n";
    }
    for (const auto& it : r_table) {
        std::string host_id = it.first;
        Path other_path = *it.second;
        float curr_delay = other_path.getTotalDelay();
        std::string link_id = other_path.getNextLink();
        std::string path_str = other_path.to_string();
        
        std::string host_row_str =
            "Host: " + host_id + "\nLink_ID: " + link_id + "\nLink_delay: " + std::to_string(curr_delay) + "\nOther Path: " + path_str + "\n";
        
        myFile << host_row_str + "\n";
    }
    myFile << "===============\n";
    myFile.close();
}

std::string Router::toString() {
    std::stringstream str;
    str << "Router: " + this->getID() + "\n";
    str << "Size of table: " << std::to_string(routing_table.size()) + "\n";
    for (const auto& q : debug_links) {
        str << q->getID().c_str() << " delay: " << std::to_string(q->getTotalDelay(uuid)) + "\n";
    }
    
    for (const auto& it : routing_table) {
        std::string host_id = it.first;
        Path other_path = *it.second;
        float curr_delay = other_path.getTotalDelay();
        std::string link_id = other_path.getNextLink();
        std::string path_str = other_path.to_string();
        
        std::string host_row_str =
            "Host: " + host_id + "\nLink_ID: " + link_id + "\nLink_delay: " + std::to_string(curr_delay) + "\nOther Path: " + path_str + "\n";
        
        str << host_row_str + "\n";
    }
    return str.str();
}
 
