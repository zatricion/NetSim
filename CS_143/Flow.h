#ifndef __CS_143_Flow__
#define __CS_143_Flow__

#include "CongestionAlg.h"

class Flow {
public:
        // Host that owns the flow
        Host *host;
        // unique id of the flow on the host.
	std::string id;
	// Source of the flow
	std::string source;
	// Destination of the flow
	std::string destination;
	// Congestion algorithm associated with the flow.
	CongestionAlg *a;
	// Total number of packets
	int numPackets; 
        // Set of all acknowledged packets.
        std::unordered_set<int> acknowledgedPackets;
        // Packets that represent the data flow.
        std::queue<Packet> flow;
        // How long the flow will wait before sending another packet.
        float waitTime;

	// Methods:
	// Constructor
        Flow(std::string id, std::string src, std::string dest, 
             CongestionAlgorithm *alg, int numPckts, int packetSize, Host *host,
             int winSize, float ts);
        // Called when there is a potentially unacknowledged event.
        // I.e. when we send a packet, we add an UnackEvent to the event heap.  Then,
        // when the event is handled, we check to see if it has been acknowledged.  If so,
        // we have a no-op.  If not, resend.
        void handleUnackEvent(Packet unacked, float time);
        // Called when an ack is received.
        void handleAck(Packet p, float time);
};

#endif /* defined(__CS_143__CongestionAlg__) */
