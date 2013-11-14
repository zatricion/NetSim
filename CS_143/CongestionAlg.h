#ifndef __CS_143__CongestionAlg__
#define __CS_143__CongestionAlg__

#include "UnackEvent.h"
#include "PacketEvent.h"
#include "Flow.h"

class CongestionAlg
{
public:
    // TODO these names suck.
    void initialize(Flow* flow);
    void handleUnackEvent(std::shared_ptr<Flow> flow, Packet& unacked, float time);
    void handleAck(std::shared_ptr<Flow> flow, Packet& pkt, float time);
};

#endif /* defined(__CS_143__CongestAlg__) */
