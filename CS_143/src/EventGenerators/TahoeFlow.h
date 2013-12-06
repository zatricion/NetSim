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
    float winOverFlow;

    // Counter for TCPReno.
    //int multiplicity;

    // Counter for TCPReno.
    //int frCount;

    // Counter for TCPReno.
    //int cavCount;

    // Counter for TCPReno.
    //int fastWindowEnd;

    // Vegas only.
    //float vegasConstAlpha;
    //float vegasConstBeta;
    //float minRTT;

    // Constructors
    TahoeFlow(std::string idval, std::string dest,
             int data_size, std::shared_ptr<Host> h,
             int winSize, float ts);

    // Overrides
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, float time) override;

    // Called when an ack is received.
    virtual void handleAck(std::shared_ptr<Packet> pkt, float time) override;

    virtual std::string toString() override;

    virtual void logFlowRTT(float time, float RTT) override;
    virtual void logFlowWindowSize(float time, int windowSize) override;

    virtual void openConnection(float time) override;
    virtual void closeConnection(float time) override;
    virtual void sendAndQueueResend(std::shared_ptr<Packet> pkt, float time, float delay) override;
    virtual void respondToSynUnackEvent(float time) override;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, float time) override;
    virtual void send(std::shared_ptr<Packet> pkt, float time) override;



};

#endif /* defined(__CS_143_TahoeFlow__) */
