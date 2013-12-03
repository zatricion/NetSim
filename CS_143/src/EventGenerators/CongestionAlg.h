#ifndef __CS_143__CongestionAlg__
#define __CS_143__CongestionAlg__

#include "../EventHandling/UnackEvent.h"
#include "../EventHandling/PacketEvent.h"
#include "Flow.h"
#include "Host.h"
#include <cassert>

class CongestionAlg
{
public:
    // TODO these names suck.
    void initialize(Flow* flow, float time);
    void sendManyPackets(Flow* flow, float time);
    virtual void handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) = 0;
    virtual void handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) = 0;
    std::string toString();
};

#endif /* defined(__CS_143__CongestAlg__) */
