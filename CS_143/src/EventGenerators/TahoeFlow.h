#ifndef __CS_143_TahoeFlow__
#define __CS_143_TahoeFlow__
#include <string>
#include <set>
#include <memory>
#include <queue>
#include "../EventHandling/Packet.h"
#include "../Tools/Log.h"
#include "Flow.h"

class TahoeFlow : public Flow {
public:
    // The phase that the TCP Tahoe alg. is in.
    TahoePhase tahoePhase;

    // Counter for TCPReno.
    int ssthresh;
    int multiplicity;
    double winOverFlow;
    double validUnackTime;

    // Counter for TCPReno.
    //int multiplicity;

    // Counter for TCPReno.
    //int frCount;

    // Counter for TCPReno.
    //int cavCount;

    // Counter for TCPReno.
    //int fastWindowEnd;

    // Vegas only.
    //double vegasConstAlpha;
    //double vegasConstBeta;
    //double minRTT;

    // Constructors
    TahoeFlow(std::string idval, std::string dest,
             int data_size, std::shared_ptr<Host> h,
             int winSize, double ts);

    // Overrides
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, double time) override;

    // Called when an ack is received.
    virtual void handleAck(std::shared_ptr<Packet> pkt, double time) override;

    virtual std::string toString() override;

    virtual void logFlowRTT(double time, float RTT) override;
    virtual void logFlowWindowSize(double time, int windowSize) override;

    virtual void openConnection(double time) override;
    virtual void closeConnection(double time) override;
    virtual void respondToSynUnackEvent(double time) override;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) override;



};

#endif /* defined(__CS_143_TahoeFlow__) */
