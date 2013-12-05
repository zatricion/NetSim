#ifndef __CS_143_TCPVegas__
#define __CS_143__TCPVegas__
#include "CongestionAlg.h"
/*
// TODO
#include "../EventHandling/UnackEvent.h"
#include "../EventHandling/PacketEvent.h"
#include "Flow.h"
#include "Host.h"
#include <cassert>
*/

class TCPVegas : public CongestionAlg
{
public:
    void handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) override;
    void handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) override;
    void handleVegasUpdate(Flow* flow, float time);
    std::string toString() override;
};

#endif /* defined(__CS_143__TCP_Vegas__) */
