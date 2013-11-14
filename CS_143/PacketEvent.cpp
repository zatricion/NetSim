#include "PacketEvent.h"

// Constructor

PacketEvent::PacketEvent(std::string dest, std::string src, float ts, std::shared_ptr<Packet> pkt) : Event(dest, src, ts), packet(pkt) {
    packet = pkt;
}

void PacketEvent::printEvent()
{
    fprintf(stdout, "\nPKT EVENT\n");
    fprintf(stdout, "------------------------\n");
    fprintf(stdout, "Source: %s\n", source.c_str());
    fprintf(stdout, "Destination: %s\n\n", destination.c_str());
    //pkt.printPacket();
}

std::string PacketEvent::getType()
{
    return "PACKET_EVENT";
}
