#include "UnackEvent.h"

// Constructor

UnackEvent::UnackEvent(std::shared_ptr<Packet> pkt,
		       std::string dest, std::string src, 
		       float ts) : Event(dest, src, ts) {
    packet = pkt;
}

void UnackEvent::printEvent()
{
    fprintf(stdout, "\nUNACK EVENT\n");
    fprintf(stdout, "------------------------\n");
    fprintf(stdout, "Source: %s\n", source.c_str());
    fprintf(stdout, "Destination: %s\n\n", destination.c_str());
}

std::string UnackEvent::getType()
{
    return "UNACK_EVENT";
}
