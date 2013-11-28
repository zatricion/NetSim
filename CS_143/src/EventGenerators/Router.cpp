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
            std::vector<std::string> path {it->getID()};
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
        float delay = it.second->getTotalDelay();
    }
    
    for (const auto& it : links) {
        // get connected router
        std::string other_node = it.second->getOtherNode(this->getID());
        auto pkt = std::make_shared<Packet>("bf_pkt_" + this->getID(), other_node, this->getID(), BF_PKT_SIZE, false, 0, "NONE", true, routing_table);
        auto bf_event = std::make_shared<PacketEvent>(it.first, this->getID(), timestamp, pkt);
        eventHeap.push(bf_event);
        
//        std::ofstream myFile;
//        myFile.open("router1.txt", std::ios::out | std::ios::app);
//        if (this->getID() == "router1")
//            for (const auto& it : routing_table) {
//                std::string host_id = it.first;
//                float curr_delay = std::get<1>(routing_table[host_id]);
//                std::string link_id = std::get<0>(routing_table[host_id]);
//                std::vector<std::string> other_path = std::get<2>(it.second);
//                std::string path = "";
//                for (const auto& p : other_path){
//                    path += p + " ";
//                }
//                std::string host_row_str =
//                "Host: " + host_id + "\nLink_ID: " + link_id + "\nLink_delay: " + std::to_string(curr_delay) + "\nOther Path: " + path + "\n";
//                myFile << host_row_str + "\n";
//            }
//        myFile.close();
        

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

void Router::updateRouting(Packet::bf_type bf_table, std::string link_id, std::string router) {
    float link_delay = links[link_id]->getTotalDelay();
    
    // DEBUG
    std::ofstream myFile;
    myFile.open("link_test.txt", std::ios::out | std::ios::app);
    // DEBUG
    printRouting(routing_table, this->getID() + "_before");
    for (const auto& it : bf_table) {
        std::string host_id = it.first;
        float total_delay = link_delay + std::get<1>(it.second);
        //if (this->getID() == "router1" && host_id=="host2") {
        //    std::cout << "Total router1->host2 delay: " << std::get<1>(it.second) << std::endl;
        //}
        float curr_delay = std::get<1>(routing_table[host_id]);
        
        //printf("Checking Routing Tables\n");
        if (total_delay < curr_delay) {
//            printf("Updating Routing Table\n");
            // check to see if current router is already in other router path
            // to avoid cycles
            std::vector<std::string> other_path = std::get<2>(it.second);
            //if (std::find(other_path.begin(), other_path.end(), this->getID()) == other_path.end()) {
            if (std::find(other_path.begin(), other_path.end(), link_id) == other_path.end()) {
                // create new path
                other_path.push_back(link_id);
                // replace old route with new route
                addRouting(host_id, link_id, total_delay, other_path);
            }
        }
        
//        // DEBUG
//        if (this->getID() == "router1" && host_id == "host2") {
//        std::string r_str = "Router: " + this->getID() + "\n";
//        std::string delay_str =
//            "Host: " + host_id + " Link: " + std::get<0>(routing_table["host2"]) + " Old Link_delay " + std::to_string(curr_delay) + " Alt Delay " + std::to_string(total_delay) + "\n";
//        myFile << r_str;
//        myFile << delay_str;
//        }
//        // DEBUG
        
    }
    
    printRouting(bf_table, router);
    printRouting(routing_table, this->getID() + "_after");
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
        if (this->getID() == "router1") {
            //std::cout << "Router1:Host 2 Link_ID " << std::get<0>(routing_table["host2"]) << " Link Delay " << std::get<1>(routing_table["host2"]) << std::endl;
        }
        updateRouting(pkt->bf_table, packet_event.source, pkt->source);
        
//        // DEBUG
//        std::ofstream myFile;
//        myFile.open("router_test.txt", std::ios::out | std::ios::app);
//        std::string r_str = "Router: " + this->getID() + "\n";
//        myFile << r_str;
//        for (const auto& it : routing_table) {
//            std::string test_str = "Host " + it.first + " Next_Dest " + std::get<0>(it.second) +"\n";
//            myFile << test_str.c_str();
//        }
//        myFile.close();
//        // DEBUG
        
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

void Router::printRouting(Packet::bf_type r_table, std::string router) {
    std::ofstream myFile;
    myFile.open("routing_table_test.txt", std::ios::out | std::ios::app);
    std::string r_str = "\nRouter: " + router + "\n";
    myFile << r_str;
    myFile << "Size of table: " + std::to_string(r_table.size()) + "\n";
    for (const auto& it : r_table) {
        std::string host_id = it.first;
        float curr_delay = std::get<1>(it.second);
        std::string link_id = std::get<0>(it.second);
        std::vector<std::string> other_path = std::get<2>(it.second);
        std::string path = "";
        for (const auto& p : other_path){
            path += p + " ";
        }
        std::string host_row_str =
            "Host: " + host_id + "\nLink_ID: " + link_id + "\nLink_delay: " + std::to_string(curr_delay) + "\nOther Path: " + path + "\n";
        
        myFile << host_row_str + "\n";
    }
    myFile << "===============\n";
    myFile.close();
}

