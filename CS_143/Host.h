#ifndef __CS_143__Host__
#define __CS_143__Host__

#include <iostream>
#include <cassert>
#include "CongestionAlg.h"
#include "Device.h"
#include "Link.h"
#include "Packet.h"

// multiple queue class TODO: see if it actually works
template<typename T> class multiQueue
{
public:
    void addQueue(std::queue<T> q)
    {
        mQ.push_back(q);
    };
    
    void deleteQueue()
    {
        mQ.erase(mQ.begin() + index);
    };
    
    T pop()
    {
        assert (!mQ.empty());
        if (mQ[index].empty())
        {
            deleteQueue();
            return this->pop();
        }
        T res = mQ[index].front();
        mQ[index].pop();
        index = (index + 1) % mQ.size;
        return res;
    }
    
private:
    std::vector<std::queue<T>> mQ;
    int index;
};

class Host : public Device
{
public:
    int window_size;
    
    // Congestion Control algorithm host runs
    CongestionAlg congestion_alg;
    
    // Link this host connects to
    Link& my_link;
    
    // Constructor takes a congestion control algorithm and an id string
    Host(CongestionAlg, Link&);
    
    // Packets waiting to be sent
    multiQueue<Packet> packet_queue;
    
    // Add event to eventHeap
    void sendPacket(Packet);
    
    // React to an event
    void giveEvent(std::unique_ptr<FlowEvent>);
    void giveEvent(std::unique_ptr<PacketEvent>);

    // Create packets for new flow and interleave them with packet_queue
    void addFlow(std::string, float, float);
    
    // Create Packet_IDS
    int packet_id;
    
};




#endif /* defined(__CS_143__Host__) */
