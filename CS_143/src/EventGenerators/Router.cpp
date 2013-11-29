#include "Router.h"
#include <fstream>

// Constructor
Router::Router(std::vector<std::string> host_list, std::vector<std::shared_ptr<Link> > neighboring_links, std::string router_id) {
    // set id
    uuid = router_id;
    
    // initialize table
    for (const auto& it : host_list) {
        Path empty_path;
        addRouting(it, empty_path);
    }
    // find adjacent hosts and add them to table
    for (const auto& it : neighboring_links) {
        std::string other_node = it->getOtherNode(this->getID());
        // if the link connects to a host
        if (std::find(host_list.begin(), host_list.end(), other_node) != host_list.end()) {
            // create path of just the link that goes to the host
            Path path = Path(it->getID());
            path.updateLinkWeight(it->getID(), it->getTotalDelay());
            addRouting(other_node, path);
        }
        addLink(it);
    }
    // add bf packetEvents to eventHeap
    broadcastTable(0.0);
}

// broadcast BF table to all connected routers
void Router::broadcastTable(float timestamp) {
    // update BF table
    for (const auto& it : links) {
        float delay = it.second->getTotalDelay();
        for (auto& bf : routing_table) {
            bf.second.updateLinkWeight(it.first, delay);
        }
    }
    // send BF table
    for (const auto& it : links) {
        // get connected router
//        if (this->getID() == "router1" && it.first == "link2") {
//            printf("Link 0 delay = %f\n", it.second->getTotalDelay());
//        }
        std::string other_node = it.second->getOtherNode(this->getID());
        auto pkt = std::make_shared<Packet>("bf_pkt_" + this->getID(), other_node, this->getID(), BF_PKT_SIZE, false, 0, "NONE", true, routing_table);
        auto bf_event = std::make_shared<PacketEvent>(it.first, this->getID(), timestamp, pkt);
        eventHeap.push(bf_event);
    }
}

// Add a link id to the router's link vector
void Router::addLink(std::shared_ptr<Link> link) {
    links[link->getID()] = link;
}

// Add a host_id -> (next_link_id, dist, path) tuple indicating which link to route to given a host, along with bellman-ford info
void Router::addRouting(std::string host_id, Path path) {
    routing_table[host_id] = path;
}

// Return the proper link to route to given a host
std::string Router::getRouting(std::string targ_host) {
    // should probably do some error checking
    return routing_table[targ_host].getNextLink();
}

void Router::updateRouting(Packet::bf_type bf_table, std::string link_id, std::string router) { // router is for debugging

    printRouting(routing_table, this->getID() + "_before");
    
    // update the delays of the links in our path before updating paths
    updateTableWeights(bf_table);
    
    // Delay of link to other router
    float link_delay = links[link_id]->getTotalDelay();
    
    for (const auto& it : bf_table) {
        std::string host_id = it.first;
        
        Path other_path = it.second;
        
        // Delay along the other path
        float other_delay = other_path.getTotalDelay();
        
        // Delay along the current path
        float curr_delay = routing_table[host_id].getTotalDelay();
        
        if (link_delay + other_delay < curr_delay) {
            // check to see if current router is already in other router path
            // to avoid cycles
            if (!other_path.hasCycle(link_id)) {
                // replace old path with new path
                other_path.addLink(link_id, link_delay);
                addRouting(host_id, other_path);
            }
        }
    }
    
    printRouting(bf_table, router);
    printRouting(routing_table, this->getID() + "_after");
}

// Deal with PacketEvents
void Router::giveEvent(std::shared_ptr<Event> e) {
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));
    
    // strip necessary info out of packet event
    std::shared_ptr<Packet> pkt = packet_event.packet;
    if (pkt->bf_tbl_bit) {
        updateRouting(pkt->bf_table, packet_event.source, pkt->source);
        broadcastTable(packet_event.eventTime() + 0.1);
    }
    else {
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
        Path this_path = routing_table[host_id];
        Path other_path = it.second;
        this_path.updateAll(other_path);
    }
}

void Router::printRouting(Packet::bf_type r_table, std::string router) {
    std::ofstream myFile;
    myFile.open("routing_table_test.txt", std::ios::out | std::ios::app);
    std::string r_str = "\nRouter: " + router + "\n";
    myFile << r_str;
    myFile << "Size of table: " + std::to_string(r_table.size()) + "\n";
    for (const auto& it : r_table) {
        std::string host_id = it.first;
        Path other_path = it.second;
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

