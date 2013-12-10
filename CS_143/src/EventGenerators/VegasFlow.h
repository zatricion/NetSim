#ifndef __CS_143_VegasFlow__
#define __CS_143_VegasFlow__
#include <string>
#include <set>
#include <memory>
#include <queue>
#include "../EventHandling/Packet.h"
#include "../Tools/Log.h"
#include "Flow.h"

class VegasFlow : public Flow {
public:
    // The phase that the TCP Reno alg. is in.
    //RenoPhase renoPhase;

    // The unsent packets.

    // Counter for TCPReno.
    //int ssthresh;

    // Counter for TCPReno.
    //int multiplicity;

    // Counter for TCPReno.
    //int frCount;

    // Counter for TCPReno.
    //int cavCount;

    // Counter for TCPReno.
    //int fastWindowEnd;

    // Vegas only.
    double vegasConstAlpha;
    double vegasConstBeta;
    double minRTT;
    double RTT;

    // Constructors
    VegasFlow(std::string idval, std::string dest,
             int data_size, std::shared_ptr<Host> h,
             int winSize, double ts);

    
    //void handleRenoUpdate(int cavCount, double time);
    void handleVegasUpdate(double time);
    //void handleTimeout(int frCount, double time);


    // Overrides
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, double time) override;

    // Called when an ack is received.
    virtual void handleAck(std::shared_ptr<Packet> pkt, double time) override;

    virtual std::string toString() override;

    virtual void closeConnection(double time) override;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) override;



};

#endif /* defined(__CS_143_VegasFlow__) */
