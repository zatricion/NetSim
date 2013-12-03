#include "Link.h"


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
    buffer_occupancy = 0;
    updateTime = 0.0;
    tempLR = -1.0;
}


float Link::getTotalDelay() {
    return queue_delay + prop_delay;
}

std::string Link::getOtherNode(std::string my_node) {
    return (my_node == node1) ? node2 : node1;
}

void Link::logLinkRate(float time) {
    float total_bits = 0.0;
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
            
            // add to total_bits sent in this period of time
            total_bits += bits_sent;
        }
    }
    
    // calculate link rate
    float link_rate = (total_bits / (time - link_time)); // / pow(10, 6);
    
    // add the link rate to the plotter

    if (time - updateTime < .005) {
        tempLR = std::max(tempLR, link_rate);
    }
    else {
        updateTime = time;
        tempLR = -1.0;
        sim_plotter.logLinkRate(this->getID(),
                            std::make_tuple(time, link_rate));
    }
}

// Log queue size
void Link::logBufferOccupancy(float time, float queue_size) {
    //if (this->getID() == "link1") {
        //std::cout << "buffer_size: " << queueKbps << std::endl;
    //}
    
    sim_plotter.logBufferOccupancy(this->getID(),
                                   std::make_tuple(time, queue_size));
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
        

        
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Log the link rate
            logLinkRate(now);
            
            // Log queue size
            logBufferOccupancy(now, queue_size);
        }
    }
    
    else {
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Packet has been dropped
            num_dropped++;
            if (now - dropped_time > 0.1) {
                sim_plotter.logPacketLoss(this->getID(), std::make_tuple(now, num_dropped));
                num_dropped = 0;
                dropped_time = now;
            }
        }
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
