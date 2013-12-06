#include "Link.h"


// Link Methods

// Constructor
Link::Link(float buf_size, float p_delay, float cap, std::string n1, std::string n2, std::string link_id)
{
    prop_delay1 = p_delay;
    prop_delay2 = p_delay;
    capacity = cap;
    node1 = n1;
    node2 = n2;
    uuid = link_id;
    buffer1_size = buf_size;
    buffer2_size = buf_size;
    link1_time = 0;
    link2_time = 0;
    queue1_delay = 0;
    queue2_delay = 0;
    queue1_size = 0;
    queue2_size = 0;
    buffer1_occupancy = 0;
    buffer2_occupancy = 0;

    FILE_LOG(logDEBUG) << "made, n1=" << n1 << ", n2=" << n2;
    FILE_LOG(logDEBUG) << "made, node1=" << node1 << ", node2=" << node2;
}


float Link::getTotalDelay() {
    //return queue_delay + prop_delay;
    return -1.0; // TODO reimplement.
}

std::string Link::getOtherNode(std::string my_node) {
    return (my_node == node1) ? node2 : node1;
}

void Link::logLinkRate(float time) {
    // TODO for now we only log one link rate.
    float total_bits = 0.0;
    if (packets_on_link1.empty()) {
        return;
    }
    auto it = packets_on_link1.begin();
    for( ; it != packets_on_link1.end(); ++it) {
        int packet_size = std::get<0>(*it);
        float on_link1_time = std::get<1>(*it);
        float off_link1_time = std::get<2>(*it);
        
        // If the previous time we logged this is greater than the off_link time,
        // just remove this
        if (off_link1_time < link1_time) {
            it = packets_on_link1.erase(it);
        }
        // Otherwise we add the number of bits sent between now and link_time to our counter
        else if (on_link1_time <= time) {
            float start = std::max(link1_time, on_link1_time);
            float end = std::min(time, off_link1_time);
            
            float bits_sent = packet_size * ((end - start) / (off_link1_time - on_link1_time));
            
            // add to total_bits sent in this period of time
            total_bits += bits_sent;
        }
    }
    
    // calculate link rate
    float link1_rate = (total_bits / (time - link1_time)); // / pow(10, 6);
    
    // add the link rate to the plotter
    sim_plotter.logLinkRate(this->getID(),
                            std::make_tuple(time, link1_rate));
    return;
}

// Log queue size
void Link::logBufferOccupancy(float time, float queue_size) {
    // TODO just do 1 for now
    sim_plotter.logBufferOccupancy(this->getID(),
                            std::make_tuple(time, queue1_size));
    return;  // TODO need to think about this.
}

void Link::giveEvent(std::shared_ptr<Event> e)
{
    FILE_LOG(logDEBUG) << "Link got a packet event.";
    // Get PacketEvent
    PacketEvent packet_event = *(std::static_pointer_cast<PacketEvent>(e));

    std::string source = packet_event.source;
    float now = packet_event.eventTime();
    
    // Queue size in bits
    if (source == node1) {
        queue1_size = std::max<float>(0, queue1_size - (now - link1_time) * capacity);
    }
    else {
        FILE_LOG(logDEBUG) << "source=" << source;
        FILE_LOG(logDEBUG) << "node2=" << node2;
        FILE_LOG(logDEBUG) << "node1=" << node1;
        assert (source == node2);
        queue2_size = std::max<float>(0, queue2_size - (now - link2_time) * capacity);
    }


    // TODO combine these statements in a nice way.  Use the helper function!

if (source == node1) {    
    // Read: if the new packet will fit in the buffer...
    if (queue1_size + packet_event.packet->size <= buffer1_size)
    {
        FILE_LOG(logDEBUG) << "PACKET NOT DROPPED.  Journeying towards " << node2;
        // Find the appropriate queue_delay.  TODO why is this a stored var?
        // I think that is kind of dangerous.
        queue1_delay = (queue1_size + packet_event.packet->size) / capacity;

        float timestamp = now + prop_delay1 + queue1_delay;
        
        // Figure out the destination
        assert (source == node1 || source == node2);
        std::string destination = (source == node1) ? node2 : node1;
        
        // Add an event to the Link priority queue
        FILE_LOG(logDEBUG) << "Pushing to queue at time=" << timestamp;
        auto packetEvent = std::make_shared<PacketEvent>(destination, uuid, timestamp, packet_event.packet);
        eventHeap.push(packetEvent);
        
        // Add packet to packets_on_link
        packets_on_link1.push_back(std::make_tuple(packet_event.packet->size, now + queue1_delay, timestamp));
        
        // Update queue size
        queue1_size += packet_event.packet->size;
        

        
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Log the link rate
            logLinkRate(now);
            
            // Log queue size
            logBufferOccupancy(now, queue1_size);
        }
    }
    
    else {
        FILE_LOG(logDEBUG) << "Packet dropped.";
        FILE_LOG(logDEBUG) << "queue1_size=" << queue1_size << ", packet_event.packet->size =" << packet_event.packet->size << ", buffer1_size=" << buffer1_size;
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Packet has been dropped
            num_dropped1++;
            if (now - dropped_time1 > 0.1) {
                sim_plotter.logPacketLoss(this->getID(), std::make_tuple(now, num_dropped1));
                num_dropped1 = 0;
                dropped_time1 = now;
            }
        }
    }
    
    // Update link_time
    link1_time = now;
}
else {
    assert(source == node2);
     // Read: if the new packet will fit in the buffer...
    if (queue2_size + packet_event.packet->size <= buffer2_size)
    {
        // TODO of course the logs aren't correct.  You're still logging the same place."
        FILE_LOG(logDEBUG) << "PACKET NOT DROPPED.  Journeying towards " << node1;
        // Find the appropriate queue_delay.  TODO why is this a stored var?
        // I think that is kind of dangerous.
        queue2_delay = (queue2_size + packet_event.packet->size) / capacity;

        float timestamp = now + prop_delay2 + queue2_delay;
        
        // Figure out the destination
        assert (source == node1 || source == node2);
        std::string destination = (source == node1) ? node2 : node1;
        
        // Add an event to the Link priority queue
        FILE_LOG(logDEBUG) << "Pushing to queue at time=" << timestamp;
        auto packetEvent = std::make_shared<PacketEvent>(destination, uuid, timestamp, packet_event.packet);
        eventHeap.push(packetEvent);
        
        // Add packet to packets_on_link
        packets_on_link2.push_back(std::make_tuple(packet_event.packet->size, now + queue2_delay, timestamp));
        
        // Update queue size
        queue2_size += packet_event.packet->size;
        

        
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Log the link rate
            logLinkRate(now);
            
            // Log queue size
            logBufferOccupancy(now, queue2_size);
        }
    }
    
    else {
        FILE_LOG(logDEBUG) << "PACKET DROPPED.";
        FILE_LOG(logDEBUG) << "queue2_size=" << queue2_size << ", packet_event.packet->size =" << packet_event.packet->size << ", buffer2_size=" << buffer2_size;
        if (this->getID() == "link1" || this->getID() == "link2"){
            // Packet has been dropped
            FILE_LOG(logDEBUG) << "Packet dropped.";
            num_dropped2++;
            if (now - dropped_time2 > 0.1) {
                sim_plotter.logPacketLoss(this->getID(), std::make_tuple(now, num_dropped2));
                num_dropped2 = 0;
                dropped_time2 = now;
            }
        }
    }
    
    // Update link_time
    link2_time = now;
}
   
    
}


float Link::getPropDelay() {
    return -1.0; //prop_delay;
}

std::string Link::toString() {
    std::stringstream fmt;
    //fmt << "{LINK: uuid=" << uuid << ", prop_delay=" << prop_delay << ", capacity=" << capacity << ", node1=" << node1 << ", node2=" << ", buffer_size=" << buffer_size << "}";
    //return fmt.str();
    return uuid;
}
