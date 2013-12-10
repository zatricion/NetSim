#include "Link.h"

/**
 * Constructor for Link.
 *
 * @param buf_size the size of the link buffers.
 * @param p_delay the propagation delay of the link.
 * @param cap the link capacity.
 * @param n1 a device connected to the link.
 * @param n2 a device connected to the link.
 * @param link_id the name of the link.
 */
Link::Link(double buf_size, float p_delay, float cap, std::string n1, std::string n2, std::string link_id)
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

/**
 * Gets the total delay of the link seen by a given sending node.
 *
 * @param node the node seeing the delay.
 * @return total delay
 */
double Link::getTotalDelay(std::string node) {
    return prop_delay + queue_delay[node];
}

/**
 * Gets the other node connected to a link.
 *
 * @param node the node we know about.
 * @return the node on the opposite end.
 */
std::string Link::getOtherNode(std::string my_node) {
    assert((my_node == node1) || (my_node == node2));
    return (my_node == node1) ? node2 : node1;
}

/**
 * Logs the link rate at a given time for a given sending node.
 *
 * @param node the node sending packets down the link.
 */
void Link::logLinkRate(double time, std::string node) {
    double total_bits = 0.0;
    double prev_time = link_rate_time[node];
    auto packets_on_this_link = packets_on_link[node];
    
    if (packets_on_this_link.empty()) {
        return;
    }
    auto it = packets_on_this_link.begin();
    for( ; it != packets_on_this_link.end(); ++it) {
        int packet_size = std::get<0>(*it);
        double on_link_time = std::get<1>(*it);
        double off_link_time = std::get<2>(*it);
        
        // If the previous time we logged this is greater than the off_link time,
        // just remove this
        if (off_link_time < prev_time) {
            it = packets_on_this_link.erase(it);
        }
        // Otherwise we add the number of bits sent between now and link_time to our counter
        else if (on_link_time <= time) {
            double start = std::max(prev_time, on_link_time);
            double end = std::min(time, off_link_time);
            
            double bits_sent = packet_size * ((end - start) / (off_link_time - on_link_time));
            
            // add to total_bits sent in this period of time
            total_bits += bits_sent;
        }
    }
    
    // calculate link rate
    double link_rate = (total_bits / (time - prev_time)); // / pow(10, 6);
    
    // add the link rate to the plotter
    sim_plotter.logLinkRate(this->getID(),
                            std::make_tuple(time, link_rate));
}

/**
 * Logs the buffer occupancy at a given time for a given sending node.
 *
 * @param node the node sending packets down the link.
 */
void Link::logBufferOccupancy(double time, std::string node) {
    sim_plotter.logBufferOccupancy(this->getID(),
                            std::make_tuple(time, queue_size[node]));
}

/**
 * Give the link an event. This method assumes that it is a packet event.
 * The link responds by determining whether the packet is dropped or not.
 * If the packet is not dropped, it is placed onto the queue for transmission.
 *
 * @param e the packet event given to the link.
 */
void Link::giveEvent(std::shared_ptr<Event> e)
{
    FILE_LOG(logDEBUG) << "Link got a packet event.";
    
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));

    std::string source = packet_event.source;
    double now = packet_event.eventTime();
    
    FILE_LOG(logDEBUG) << "source=" << source;
    FILE_LOG(logDEBUG) << "node2=" << node2;
    FILE_LOG(logDEBUG) << "node1=" << node1;
    
    // Queue size in bits
    queue_size[source] = std::max<double>(0.0, queue_size[source] - (now - link_time[source]) * capacity);
    
    // std::cout << queue_size[source] << std::endl;

    double this_queue_size = queue_size[source];
    std::string other_node = getOtherNode(source);

    // Read: if the new packet will fit in the buffer...
    if (this_queue_size + packet_event.packet->size <= buffer_size)
    {
        FILE_LOG(logDEBUG) << "PACKET NOT DROPPED.  Journeying towards " << other_node;
        // Update the appropriate queue_delay.
        queue_delay[source] = (this_queue_size + packet_event.packet->size) / capacity;

        double timestamp = now + prop_delay + queue_delay[source];
        
        // Add an event to the Link priority queue
        FILE_LOG(logDEBUG) << "Pushing to queue at time=" << timestamp;
        auto packetEvent = std::make_shared<PacketEvent>(other_node, uuid, timestamp, packet_event.packet);
        eventHeap.push(packetEvent);
        
        // Add packet to packets_on_link
        packets_on_link[source].push_back(std::make_tuple(packet_event.packet->size, now + queue_delay[source], timestamp));
        
        // Update queue size
        queue_size[source] += packet_event.packet->size;
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
    
    
    if ((this->getID() == "link1") || (this->getID() == "link2") || (this->getID() == "link3")) {
        // Log only data packets so that we see only one direction (assumes all flows go right)
        if ((packet_event.packet->bf_tbl_bit == false)
            && (packet_event.packet->ack == false)
            && ((now - link_rate_time[source]) > 0.1)) {
            
            // Log the link rate
            logLinkRate(now, source);
            
            link_rate_time[source] = now;
            
            // Log queue size
            logBufferOccupancy(now, source);
        }
    }

    
    // Update link_time
    link_time[source] = now;
}

/**
 * Gets the propagation delay of a link.
 *
 * @return the propagation delay of the link.
 */
double Link::getPropDelay() {
    return prop_delay;
}

/**
 * Represents the link as a string.
 *
 * @return the string representing the link.
 */
std::string Link::toString() {
    std::stringstream fmt;
    fmt << "{LINK: uuid=" << uuid << ", prop_delay=" << prop_delay << ", capacity=" << capacity << ", node1=" << node1 << ", node2=" << ", buffer_size=" << buffer_size << "}";
    return fmt.str();
}
