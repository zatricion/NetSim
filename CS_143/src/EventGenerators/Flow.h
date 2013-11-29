#ifndef __CS_143_Flow__
#define __CS_143_Flow__
#include <string>
#include <set>
#include <memory>
#include <queue>
#include <memory>
#include "../EventHandling/Packet.h"
#include "../Tools/Log.h"


class CongestionAlg;
class Host;

enum Phase {SYN, DATA, FIN, DONE};
enum RenoPhase {SLOWSTART, CONGESTIONAVOIDANCE, FASTRECOVERY};
// packet size is 100KB
static const int DATA_PKT_SIZE = 100000;

class Flow
{
public:
    // Using go back N, this is the start of the window (inclusive).
    int windowStart;

    // Using go back N, this is the end of the window (inclusive).
    int windowEnd;
    // Host that owns the flow
    std::shared_ptr<Host> host;

    // ID of the flow
    std::string id;

    // source of the flow.  Should equal host->uuid
    std::string source;

    // Destination of the flow
    std::string destination;

    // Congestion algorithm associated with the flow.
    std::shared_ptr<CongestionAlg> a;

    // Total number of packets in the flow.
    int numPackets;

    // How long the flow will wait before sending another packet.
    float waitTime;

    // packet size
    int packetSize;

    // Time when the flow is generated.
    float timestamp;

    // The current phase we are in.
    Phase phase;

    // The phase that the TCP Reno alg. is in.
    RenoPhase renoPhase;

    // The unsent packets.
    std::set<int> unSentPackets;

    // Counter for TCPReno.
    int ssthresh;

    // Counter for TCPReno.
    int multiplicity;

    // Counter for TCPReno.
    int frCount;

    // Counter for TCPReno.
    int cavCount;

    // Counter for TCPReno.
    int fastWindowEnd;

    // Constructors
    Flow(std::string idval, std::string dest,
             std::shared_ptr<CongestionAlg> alg, int data_size, std::shared_ptr<Host> h,
             int winSize, float ts);
    
    // Called when there is a potentially unacknowledged event.
    // When we send a packet, we add an UnackEvent to the event heap.  Then,
    // when the event is handled, we check to see if it has been acknowledged.
    // If so, we have a no-op.  If not, resend.
    void handleUnackEvent(std::shared_ptr<Packet> unacked, float time);
    void handleRenoUpdate(int cavCount, float time);
    void handleTimeout(int frCount, float time);
    
    // Called when an ack is received.
    void handleAck(std::shared_ptr<Packet> p, float time);

    std::string toString();
        
    void initialize(float time);
};

#endif /* defined(__CS_143_Flow__) */
