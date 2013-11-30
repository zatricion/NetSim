#include "Router.h"

// Add a link id to the router's link vector
void Router::addLink(std::string link_id) {
    links.push_back(link_id);
}

// Add a (host, link) pair indicating which link to route to given a host
void Router::addRouting(std::string targ_host, std::string targ_link) {
  routing_table[targ_host] = targ_link;
}

// Return the proper link to route to given a host
std::string Router::getRouting(std::string targ_host) {
    // should probably do some error checking
    return routing_table[targ_host];
}

// Make new event with same packet going to proper link
void Router::giveEvent(std::shared_ptr<Event> e) {
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));
    
    // strip necessary info out of packet event 
    std::shared_ptr<Packet> pkt = packet_event.packet;
    std::string dest = getRouting(pkt->final_dest);
    // make new event
    auto new_event = std::make_shared<PacketEvent>(dest, this->getID(), packet_event.eventTime(), pkt);
    // put on event heap
    eventHeap.push(new_event);
}

std::string Router::toString() {
    std::stringstream fmt;
    fmt << "{Router: uuid=" << uuid << "}";
    return fmt.str();
}
