#include "Link.h"
#include <cassert>
#include <iostream>
#include <algorithm> // std::max
#include <sstream>

// Link Methods

// Constructor
Link::Link(float buf_size, float p_delay, float cap, std::string n1, std::string n2, std::string link_id)
{
    prop_delay = p_delay;
    capacity = cap;
    node1 = n1;
    node2 = n2;
    uuid = link_id;
    buffer_size = buf_size;
    link_time = 0;
    queue_delay = 0;
    queue_size = 0;
}


float Link::getTotalDelay() {
    return queue_delay + prop_delay;
}

std::string Link::getOtherNode(std::string my_node) {
    return (my_node == node1) ? node2 : node1;
}

void Link::logLinkRate(float time) {
    if (packets_on_link.empty()) {
        return;
    }
    auto it = packets_on_link.begin();
    for( ; it != packets_on_link.end(); ++it) {
        int packet_size = std::get<0>(*it);
        float on_link_time = std::get<1>(*it);
        float off_link_time = std::get<2>(*it);
        
        // If the previous time we logged this is greater than the off_link time,
        // just remove this
        if (off_link_time < link_time) {
            it = packets_on_link.erase(it);
        }
        // Otherwise we add the number of bits sent between now and link_time to our counter
        else if (on_link_time <= time) {
            float start = std::max(link_time, on_link_time);
            float end = std::min(time, off_link_time);
            
            float bits_sent = packet_size * ((end - start) / (off_link_time - on_link_time));
            
            // calculate link rate
            float link_rate = bits_sent / (time - link_time);
            
            // TODO: Johno, log this instead of returning
            // return std::make_tuple(link_rate, time);
        }
    }

}

void Link::giveEvent(std::shared_ptr<Event> e)
{
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));

    std::string source = packet_event.source;
    float now = packet_event.eventTime();
    
    // Queue size in bits
    queue_size = std::max<float>(0, queue_size - (now - link_time) * capacity);
    if (queue_size + packet_event.packet->size < buffer_size)
    {
        queue_delay = (queue_size + packet_event.packet->size) / capacity;
//        if (this->getID() == "link0")
//            std::cout << "Link 0 Delay " << getTotalDelay() << " Time: " << now << std::endl;
        
        // Add propagation and queue delay to current time to get event time
        float timestamp = now + prop_delay + queue_delay;
        
        // Figure out the destination
        assert (source == node1 || source == node2);
        std::string destination = (source == node1) ? node2 : node1;
        
        // Add an event to the Link priority queue
        auto packetEvent = std::make_shared<PacketEvent>(destination, uuid, timestamp, packet_event.packet);
        eventHeap.push(packetEvent);
        
        // Add packet to packets_on_link
        packets_on_link.push_back(std::make_tuple(packet_event.packet->size, now + queue_delay, timestamp));
        
        // Update queue size
        queue_size += packet_event.packet->size;
        
        // Log the link rate
        logLinkRate(now);
    }
    
    else
    {
        // TODO: Johno, Log dropped pkts for each link
    }
    
    // Update link_time
    link_time = now;
}

float Link::getPropDelay() {
    return prop_delay;
}

std::string Link::toString() {
    std::stringstream fmt;
    fmt << "{LINK: uuid=" << uuid << ", prop_delay=" << prop_delay << ", capacity=" << capacity << ", node1=" << node1 << ", node2=" << ", buffer_size=" << buffer_size << "}";
    return fmt.str();
}
