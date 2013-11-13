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

void Router::giveEvent(std::unique_ptr<Event>) {};

// Make new event with same packet going to proper link
void Router::giveEvent(std::unique_ptr<PacketEvent> packet_event) {
    // strip necessary info out of packet event 
    Packet pkt = packet_event->packet;
    std::string dest = getRouting(pkt.final_dest);
    // make new event
    PacketEvent new_event(dest, getID(), packet_event->eventTime(), pkt);
    // put on event heap
    eventHeap.push(new_event);
}
