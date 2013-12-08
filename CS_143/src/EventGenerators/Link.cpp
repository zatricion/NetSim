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
    FILE_LOG(logDEBUG) << "made, n1=" << n1 << ", n2=" << n2;
    FILE_LOG(logDEBUG) << "made, node1=" << node1 << ", node2=" << node2;
}


float Link::getTotalDelay(std::string node) {
    return prop_delay + queue_delay[node];
}

std::string Link::getOtherNode(std::string my_node) {
    assert((my_node == node1) || (my_node == node2));
    return (my_node == node1) ? node2 : node1;
}

void Link::logLinkRate(float time, std::string node) {
    float total_bits = 0.0;
    float prev_time = link_time[node];
    auto packets_on_this_link = packets_on_link[node];
    
    if (packets_on_this_link.empty()) {
        return;
    }
    auto it = packets_on_this_link.begin();
    for( ; it != packets_on_this_link.end(); ++it) {
        int packet_size = std::get<0>(*it);
        float on_link_time = std::get<1>(*it);
        float off_link_time = std::get<2>(*it);
        
        // If the previous time we logged this is greater than the off_link time,
        // just remove this
        if (off_link_time < prev_time) {
            it = packets_on_this_link.erase(it);
        }
        // Otherwise we add the number of bits sent between now and link_time to our counter
        else if (on_link_time <= time) {
            float start = std::max(prev_time, on_link_time);
            float end = std::min(time, off_link_time);
            
            float bits_sent = packet_size * ((end - start) / (off_link_time - on_link_time));
            
            // add to total_bits sent in this period of time
            total_bits += bits_sent;
        }
    }
    
    // calculate link rate
    float link_rate = (total_bits / (time - prev_time)); // / pow(10, 6);
    
    // add the link rate to the plotter
    sim_plotter.logLinkRate(this->getID(),
                            std::make_tuple(time, link_rate));
    return;
}

// Log queue size
void Link::logBufferOccupancy(float time, std::string node) {
    sim_plotter.logBufferOccupancy(this->getID(),
                            std::make_tuple(time, queue_size[node]));
}

void Link::giveEvent(std::shared_ptr<Event> e)
{
    FILE_LOG(logDEBUG) << "Link got a packet event.";
    
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));

    std::string source = packet_event.source;
    float now = packet_event.eventTime();
    
    FILE_LOG(logDEBUG) << "source=" << source;
    FILE_LOG(logDEBUG) << "node2=" << node2;
    FILE_LOG(logDEBUG) << "node1=" << node1;
    
    // Queue size in bits
    queue_size[source] = std::max<float>(0, queue_size[source] - (now - link_time[source]) * capacity);

    float this_queue_size = queue_size[source];
    std::string other_node = getOtherNode(source);

    // Read: if the new packet will fit in the buffer...
    if (this_queue_size + packet_event.packet->size <= buffer_size)
    {
        FILE_LOG(logDEBUG) << "PACKET NOT DROPPED.  Journeying towards " << other_node;
        // Update the appropriate queue_delay.
        queue_delay[source] = (this_queue_size + packet_event.packet->size) / capacity;

        float timestamp = now + prop_delay + queue_delay[source];
        
        // Add an event to the Link priority queue
        FILE_LOG(logDEBUG) << "Pushing to queue at time=" << timestamp;
        auto packetEvent = std::make_shared<PacketEvent>(other_node, uuid, timestamp, packet_event.packet);
        eventHeap.push(packetEvent);
        
        // Add packet to packets_on_link
        packets_on_link[source].push_back(std::make_tuple(packet_event.packet->size, now + queue_delay[source], timestamp));
        
        // Update queue size
        queue_size[source] += packet_event.packet->size;
        
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Log only data packets so that we see only one direction (assumes all flows go right)
            if ((packet_event.packet->bf_tbl_bit == false)
                && (packet_event.packet->ack == false)) {
                
                // Log the link rate
                logLinkRate(now, source);
                
                // Log queue size
                logBufferOccupancy(now, source);
            }
        }
    }
    
    else {
        FILE_LOG(logDEBUG) << "Packet dropped.";
        FILE_LOG(logDEBUG) << "Going from " << source << " to " << other_node;
        FILE_LOG(logDEBUG) << "queue_size=" << queue_size[source] << ", packet_event.packet->size =" << packet_event.packet->size << ", buffer_size=" << buffer_size;
        
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Packet has been dropped
            num_dropped[source]++;
            if (now - dropped_time[source] > 0.1) {
                sim_plotter.logPacketLoss(this->getID(), std::make_tuple(now, num_dropped[source]));
                num_dropped[source] = 0;
                dropped_time[source] = now;
            }
        }
    }
    
    // Update link_time
    link_time[source] = now;
}

float Link::getPropDelay() {
    return prop_delay;
}

std::string Link::toString() {
    //std::stringstream fmt;
    //fmt << "{LINK: uuid=" << uuid << ", prop_delay=" << prop_delay << ", capacity=" << capacity << ", node1=" << node1 << ", node2=" << ", buffer_size=" << buffer_size << "}";
    //return fmt.str();
    return uuid;
}
