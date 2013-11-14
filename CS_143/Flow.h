#ifndef __CS_143_Flow__
#define __CS_143_Flow__
#include <string>
#include <unordered_set>
#include <memory>
#include <queue>
#include "Packet.h"


class CongestionAlg;
class Host;

// packet size is 100KB
static const int DATA_PKT_SIZE = 100000;

class Flow
{
public:
    // Host that owns the flow
    std::shared_ptr<Host> host;
    
    // shared id of the flow on the host.
	std::string id;
    
	// Source of the flow
	std::string source;
    
	// Destination of the flow
	std::string destination;
    
	// Congestion algorithm associated with the flow.
	std::shared_ptr<CongestionAlg> a;
    
	// Total number of packets
	int numPackets;

    // Set of all acknowledged packets.
    std::unordered_set<int> acknowledgedPackets;
    
    // Packets that represent the data flow.
    std::queue<Packet> flow;
    
    // How long the flow will wait before sending another packet.
    float waitTime;
    
    // window size
    int windowSize;
    
    int packetSize;
    
    // Time when the flow is generated.  Used to create a small set
    // of Events.
    float timestamp;

	// Methods:
    
	// Constructor
    Flow();
    Flow(std::string idval, std::string src, std::string dest,
             std::shared_ptr<CongestionAlg> alg, int data_size, std::shared_ptr<Host> h,
             int winSize, float ts);
    
    // Called when there is a potentially unacknowledged event.
    // I.e. when we send a packet, we add an UnackEvent to the event heap.  Then,
    // when the event is handled, we check to see if it has been acknowledged.
    // If so, we have a no-op.  If not, resend.
    void handleUnackEvent(Packet unacked, float time);
    
    // Called when an ack is received.
    void handleAck(Packet p, float time);

    std::string toString();
        
    void initialize();
};

#endif /* defined(__CS_143_Flow__) */
