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
    double waitTime;

    // packet size
    int packetSize;

    // Time when the flow is generated.
    double timestamp;

    // The current phase we are in.
    Phase phase;

    std::set<int> unSentPackets;

    double A;
    double D;
    double b;

    // No constructors.
    Flow(std::string idval, std::string dest,
         int data_size, std::shared_ptr<Host> h,
         int winSize, double ts);


    //
    // Called when there is a potentially unacknowledged event.
    // When we send a packet, we add an UnackEvent to the event heap.  Then,
    // when the event is handled, we check to see if it has been acknowledged.
    // If so, we have a no-op.  If not, resend.
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, double time) = 0;

    // Called when an ack is received.
    virtual void handleAck(std::shared_ptr<Packet> pkt, double time) = 0;

    virtual std::string toString() = 0;

    void initialize(double time);

    virtual void logFlowRTT(double time, float RTT) = 0;
    virtual void logFlowWindowSize(double time, int windowSize) = 0;

    virtual void openConnection(double time) = 0;
    virtual void closeConnection(double time) = 0;
    void sendAndQueueResend(std::shared_ptr<Packet> pkt, double time, float delay);
    virtual void respondToSynUnackEvent(double time) = 0;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) = 0;
    void send(std::shared_ptr<Packet> pkt, double time);
    void sendManyPackets(double time);
};
#endif
