#ifndef __CS_143_Flow__
#define __CS_143_Flow__
#include <string>
#include <set>
#include <memory>
#include <queue>
#include "../EventHandling/Packet.h"
#include "../Tools/Log.h"

class CongestionAlg;
class Host;

enum Phase {SYN, DATA, FIN, DONE};
enum TahoePhase {SLOWSTART, CONGESTIONAVOIDANCE};
// data packet size is 1KB
static const int DATA_PKT_SIZE = 1024 * 8;

// other packets are 64B
static const int ACK_SIZE = 64 * 8;
static const int SYN_SIZE = 64 * 8;
static const int FIN_SIZE = 64 * 8;

class Flow {
public:
    int windowStart;
    int windowEnd;
    std::shared_ptr<Host> host;
    std::string id;
    std::string source;
    std::string destination;

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

    std::set<int> unSentPackets;

    float A;
    float D;
    float b;

    // No constructors.
    Flow(std::string idval, std::string dest,
         int data_size, std::shared_ptr<Host> h,
         int winSize, float ts);


    //
    // Called when there is a potentially unacknowledged event.
    // When we send a packet, we add an UnackEvent to the event heap.  Then,
    // when the event is handled, we check to see if it has been acknowledged.
    // If so, we have a no-op.  If not, resend.
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, float time) = 0;

    // Called when an ack is received.
    virtual void handleAck(std::shared_ptr<Packet> pkt, float time) = 0;

    virtual std::string toString() = 0;

    void initialize(float time);

    virtual void logFlowRTT(float time, float RTT) = 0;
    virtual void logFlowWindowSize(float time, int windowSize) = 0;

    virtual void openConnection(float time) = 0;
    virtual void closeConnection(float time) = 0;
    void sendAndQueueResend(std::shared_ptr<Packet> pkt, float time, float delay);
    virtual void respondToSynUnackEvent(float time) = 0;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, float time) = 0;
    void send(std::shared_ptr<Packet> pkt, float time);
    void sendManyPackets(float time);
};
#endif
