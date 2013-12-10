/**
 * Implementation of the Tahoe congestion control algorithm.
 */


#ifndef __CS_143_TahoeFlow__
#define __CS_143_TahoeFlow__
#include <string>
#include <set>
#include <memory>
#include <queue>
#include <cassert>
#include <math.h>
#include <climits>
#include "Flow.h"
#include "Host.h"
#include "../EventHandling/Packet.h"
#include "../Tools/Log.h"


/**
 * An enum for the phase of tahoe we are in.
 */
enum TahoePhase {SLOWSTART, CONGESTIONAVOIDANCE};


class TahoeFlow : public Flow {


public:
    // Fields
    /** The phase that the TCP Tahoe alg. is in. */
    TahoePhase tahoePhase;

    /** The SLOWSTART threshold. */
    int ssthresh;

    /** A counter to see how many acks we have received for a single Packet. */
    int multiplicity;

    /** A variable to hold the overflow in window size from updates. */ 
    double winOverFlow;

    /** The time of the last timeout.  See handleAck for details. */
    double validUnackTime;

    // Constructors
    TahoeFlow(std::string idval, std::string dest, int data_size,
              std::shared_ptr<Host> h, int winSize, double ts);

    // Methods
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, double time) override;
    virtual void handleAck(std::shared_ptr<Packet> pkt, double time) override;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) override;
    virtual std::string toString() override;
    void updateWaitTime();
};


#endif /* defined(__CS_143_TahoeFlow__) */
