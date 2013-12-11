/**
 * A generic object representing a data flow.  Contains methods for handling
 * acks and sending packets that are sufficiently generic to use for both
 * Tahoe and Vegas.  These algorithms use Go-Back-N style acks.
 */


#ifndef __CS_143_Flow__
#define __CS_143_Flow__
#include <string>
#include <set>
#include <memory>
#include <queue>
#include <cassert>
#include <math.h>
#include "../EventHandling/Packet.h"
#include "../Tools/Log.h"

// Needed because of circular dependency between Flow and Host.
class Host;

/**
 * The phase of the data flow.
 */
enum Phase {SYN, DATA, FIN, DONE};

/** Packet sizes, in bits. */
static const int DATA_PKT_SIZE = 1024 * 8;
/** Packet sizes, in bits. */
static const int ACK_SIZE = 64 * 8;
/** Packet sizes, in bits. */
static const int SYN_SIZE = 1024 * 8;
/** Packet sizes, in bits. */
static const int FIN_SIZE = 64 * 8;


class Flow {

public:
    // Fields
    /** The start of the send window. */
    int windowStart;

    /** The end of the send window. */
    int windowEnd;

    /**
     * A pointer to the host that represents the sending end of the data
     * transfer.
     */
    std::shared_ptr<Host> host;

    /** The ID of the flow.  IDs must be globally unique. */
    std::string id;

    /** The source of the flow.  Should be the id of the host. */
    std::string source;

    /** The destination of the flow.  Should be the id of a host. */
    std::string destination;

    /** The total number of data packets in the flow. */
    int num_packets;

    /** The time a flow waits until a packet has timed out. */
    double wait_time;

    /** The size of packets in the flow. */
    int packetSize;

    /** The time at which the flow begins. */
    double timestamp;

    /**
     * The current phase of the flow.  The flow begins in the SYN phase, then
     * moves to the DATA phase (in which data transfer occurs), and closes in
     * FIN phase.  The flow enters the DONE phase when a FINACK is received.
     */
    Phase phase;

    /**
     * A list of packets that the flow hasn't sent yet.  Initially, this will
     * be the set {0, 1, ..., num_packets - 1}.
     */
    std::set<int> unSentPackets;

    /** 
      * The time-averaged RTT.  See page 92 of "Communication Networks: A 
      * Concise Introduction" for an explanation of how this is updated.
      */
    double A;

    /** A measure of the variation of A. */
    double D;

    /** A weighting factor used in calculations of A and D.  See page 92 of
      * "Communication Networks: A Concise Introduction".
      */
    double b;

    // Constructors
    Flow(std::string idval, std::string dest,
         int data_size, std::shared_ptr<Host> h,
         int winSize, double ts);

    // Methods
    /**
     * Called when a packet may have timed out.  Some fields are checked to
     * determine whether or not the packet has been acked.  An action is taken
     * based on the result.
     *
     * @param unacked the packed that may or may not have been acknowledged.
     * the packet represents the packet inside of an UnackEvent.
     * @param time the time at which the UnackEvent is occurring.
     */
    virtual void handleUnackEvent(std::shared_ptr<Packet> unacked, 
                                  double time) = 0;

    /**
     * Called when an ack is received for a data packet that this flow sent out.
     *
     * @param pkt the ack received.
     * @param time the time at which the ack is received.
     */
    virtual void handleAck(std::shared_ptr<Packet> pkt, double time) = 0;

    /**
     * A string representing the Flow object.  Used mainly for debugging.
     */
    virtual std::string toString() = 0;

    /**
     * Respond to the receipt of a SYN for this flow.
     */
    virtual void respondToSynPacketEvent(std::shared_ptr<Packet> pkt, double time) = 0;
   
    void logFlowWindowSize(double time, int windowSize);
    void openConnection(double time);
    void initialize(double time);
    void sendAndQueueResend(std::shared_ptr<Packet> pkt, double time, float delay);
    void send(std::shared_ptr<Packet> pkt, double time);
    void sendManyPackets(double time);
    void logFlowRTT(double time, double RTT);
    void respondToSynUnackEvent(double time);
};


#endif
