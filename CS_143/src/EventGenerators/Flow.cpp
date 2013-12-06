#include "Flow.h"
#include "Host.h"
//#include "CongestionAlg.h"
#include "TCPReno.h"
#include "TCPVegas.h"
#include <cassert>
#include <math.h>
#include "../Tools/Log.h"

// Constructor.
Flow::Flow(std::string idval, std::string dest,
           int data_size,
           std::shared_ptr<Host> h, int winSize, float ts) {
    timestamp = ts;
    host = h;
    id = idval;
    source = h->getID();
    destination = dest;
    numPackets = ceil(1.0 * data_size / DATA_PKT_SIZE);
    waitTime = .5;
    phase = SYN;
    unSentPackets = std::set<int>();
    for (int i = 0; i < numPackets; i++) {
        unSentPackets.insert(i);
    }
    windowStart = 0;
    windowEnd = winSize - 1;
    //cavCount = 0;
    //ssthresh
    //multiplicity
    //frCount = 0;
    //fastWindowEnd = -1;
    A = waitTime;
    D = waitTime;
    b = .1;
}

void Flow::initialize(float time) {
    sendManyPackets(time);
}

void Flow::sendManyPackets(float time) {
    for (int i = windowStart; i <= windowEnd; i++) {
        if (unSentPackets.count(i) == 1) {
            // He has not been sent before.  Let's send him, and queue a resend.
            unSentPackets.erase(i);
            auto pkt = std::make_shared<Packet>(std::to_string(i),
                destination, source, DATA_PKT_SIZE,
                false, i, id, false, false, time);
        host->sendAndQueueResend(pkt, time, waitTime);
        }
    }
}
