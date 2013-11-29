#ifndef __CS_143_TCPReno__
#define __CS_143__TCPReno__
#include "CongestionAlg.h"
/*
// TODO
#include "../EventHandling/UnackEvent.h"
#include "../EventHandling/PacketEvent.h"
#include "Flow.h"
#include "Host.h"
#include <cassert>
*/

class TCPReno : public CongestionAlg
{
public:
    void handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) override;
    void handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) override;
    void handleTimeout(Flow* flow, int frCount, float time);
    void handleRenoUpdate(Flow* flow, int cavCount, float time);
};

#endif /* defined(__CS_143__TCP_Reno__) */
