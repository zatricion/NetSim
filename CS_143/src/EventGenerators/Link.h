#ifndef __CS_143__Link__
#define __CS_143__Link__

#include <iostream>
#include "EventGenerator.h"
#include "../EventHandling/PacketEvent.h"
#include <list>
#include "../IO/Plotter.h"
#include <cassert>
#include <algorithm> // std::max
#include <sstream>
#include "../Tools/Log.h"
#include <unordered_map>

class Link : public EventGenerator
{

public:
    Link(double, float, float, std::string, std::string, std::string);
    
    virtual void giveEvent(std::shared_ptr<Event>) override;
    
    double getTotalDelay(std::string node);
    
    std::string getOtherNode(std::string node);
    
    void logLinkRate(double time, std::string node);
    
    void logBufferOccupancy(double time, std::string node);
    
    void packetLoss(double);
    
    double getPropDelay();
    
    std::string toString();
    
private:
    // Maximum queue_size in bits
    double buffer_size;

    // Delay seen by an incoming packet due to link length
    double prop_delay;
    
    // Queue size in bits at queue_time for each queue
    std::unordered_map<std::string, double> queue_size;
    
    // Delay seen by an incoming packet due to the queue for each queue
    std::unordered_map<std::string, double> queue_delay;
    
    // Number of bits sent per time-step
    double capacity;
    
    std::string node1;
    std::string node2;
    
    // Last update time for each queue
    std::unordered_map<std::string, double> link_time;
    
    // Last time link rate was logged for each queue
    std::unordered_map<std::string, double> link_rate_time;
    
    // Tuples (pkt_size, on_link_time, off_link_time) for each queue
    std::unordered_map<std::string, std::list<std::tuple<int, double, float> > >packets_on_link;
    
    // Number of packets on queue for each queue
    std::unordered_map<std::string, int> buffer_occupancy;
    
    // Packets dropped in current bin of time for each queue
    std::unordered_map<std::string, int> num_dropped;
    
    // Last time dropped packets were logged for each queue
    std::unordered_map<std::string, double> dropped_time;
};

#endif /* defined(__CS_143__Link__) */
