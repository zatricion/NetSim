#include "Router.h"
#include <fstream>

// Constructor
Router::Router(std::vector<std::string> host_list, std::vector<std::shared_ptr<Link> > neighboring_links, std::string router_id) {
    // set id
    uuid = router_id;
    
    // initialize table
    for (const auto& it : host_list) {
        std::vector<std::string> empty_path;
        addRouting(it, "NONE", std::numeric_limits<float>::max(), empty_path);
    }
    // find adjacent hosts and add them to table
    for (const auto& it : neighboring_links) {
        std::string other_node = it->getOtherNode(this->getID());
        if (std::find(host_list.begin(), host_list.end(), other_node) != host_list.end()) {
            // create path of just this node
            std::vector<std::string> path {this->getID()};
            addRouting(other_node, it->getID(), it->getTotalDelay(), path);
        }
        // add only links that connect to routers
        else {
            addLink(it);
        }
    }
    // add bf packetEvents to eventHeap
    broadcastTable(0.0);
}

// broadcast BF table to all connected routers
void Router::broadcastTable(float timestamp) {
    for (const auto& it : links) {
        // get connected router
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
void Router::addRouting(std::string host_id, std::string next_link_id, float dist, std::vector<std::string> path) {
    routing_table[host_id] = std::make_tuple(next_link_id, dist, path);
}

// Return the proper link to route to given a host
std::string Router::getRouting(std::string targ_host) {
    // should probably do some error checking
    return std::get<0>(routing_table[targ_host]);
}

void Router::updateRouting(Packet::bf_type bf_table, std::string link_id) {
    float link_delay = links[link_id]->getTotalDelay();
    
    // DEBUG
    std::ofstream myFile;
    myFile.open("link_test.txt", std::ios::out | std::ios::app);
    // DEBUG
    
    for (const auto& it : bf_table) {
        std::string host_id = it.first;
        float total_delay = link_delay + std::get<1>(it.second);
        if (total_delay < std::get<1>(routing_table[host_id])) {
            // check to see if current router is already in other router path
            // to avoid cycles
            std::vector<std::string> other_path = std::get<2>(it.second);
            if (std::find(other_path.begin(), other_path.end(), this->getID()) == other_path.end()) {
                // create new path
                other_path.push_back(this->getID());
                // replace old route with new route
                addRouting(host_id, link_id, total_delay, other_path);
            }
        }
        
        // DEBUG
        std::string r_str = "Router: " + this->getID() + "\n";
        std::string delay_str = "Link: " + link_id + " Link_delay " + std::to_string(total_delay) + "\n";
        myFile << r_str;
        myFile << delay_str;
        // DEBUG
        
    }
    
    // DEBUG
    myFile.close();
    // DEBUG
    
}

// Deal with PacketEvents
void Router::giveEvent(std::shared_ptr<Event> e) {
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));
    
    // strip necessary info out of packet event
    std::shared_ptr<Packet> pkt = packet_event.packet;
    
    if (pkt->bf_tbl_bit) {
        updateRouting(pkt->bf_table, packet_event.source);
        
        // DEBUG
        std::ofstream myFile;
        myFile.open("router_test.txt", std::ios::out | std::ios::app);
        std::string r_str = "Router: " + this->getID() + "\n";
        myFile << r_str;
        for (const auto& it : routing_table) {
            std::string test_str = "Host " + it.first + " Next_Dest " + std::get<0>(it.second) +"\n";
            myFile << test_str.c_str();
        }
        myFile.close();
        // DEBUG
        
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

