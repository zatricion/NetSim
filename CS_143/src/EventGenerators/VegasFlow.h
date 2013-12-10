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
    // Fields
    double vegasConstAlpha;
    double vegasConstBeta;
    double minRTT;
    double RTT;

    // Constructors
    VegasFlow(std::string idval, std::string dest,
             int data_size, std::shared_ptr<Host> h,
             int winSize, double ts);

    // Methods
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, double time)
        override;
    virtual void handleAck(std::shared_ptr<Packet> pkt, double time) override;
    virtual std::string toString() override;
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt,
                                         double time) override;
    void handleVegasUpdate(double time);
    void updateWaitTime();
};


#endif /* defined(__CS_143_VegasFlow__) */
