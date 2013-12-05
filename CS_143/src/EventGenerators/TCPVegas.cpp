#include "TCPVegas.h"
#include "../EventHandling/TimeoutEvent.h"
#include "../EventHandling/TCPVegasUpdateEvent.h"
#include <algorithm>

/**
 * Called when a packet has not been acknowledged for waitTime.
 * 
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param unacked the unacknowledged packet
 * @param time the time at which the unackedEvent was thrown
 */
void TCPVegas::handleUnackEvent(Flow* flow, std::shared_ptr<Packet> unacked, float time) {
    assert(false);
}


/**
 * Called when an ack has been received for a particular flow.
 *
 * @param flow a pointer to the flow object that called the CongestionAlg
 * @param pkt the ack packet
 * @param time the time at which the event was received
 */
void TCPVegas::handleAck(Flow* flow, std::shared_ptr<Packet> pkt, float time) {
    assert(false);
}


void TCPVegas::handleVegasUpdate(Flow *flow, float time) {
    assert(false);
}

std::string TCPVegas::toString() {
    return "TCPVegas";
}
