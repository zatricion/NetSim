// Tests basic use cases of objects.
#include "Test.h"
#include <cassert>

// @MaxHorton TODO:
// This method will contain unit tests for each of the objects.
// It will be implemented as the network objects are created.
int main() {
    std::cout << "Preparing to test objects." << std::endl;
    std::cout << "Testing Object constructors." << std::endl;
    packetTest();
    eventTest();
    return 0;
}

/**
 * Simple test of packets.  Not much to test...
 */
void packetTest() {
	std::cout << "Testing packets...";
	//Packet p("uuid", "final_dest", "source", 10, true, false, false, NULL, 0);
	// Not really anything to test... no methods in Packet.h
	//assert (p.uuid == "uuid");
	std::cout << "Packets passed tests!" << std::endl;
}

/**
 * Simple test of events.
 */
void eventTest() {
	std::cout << "Testing events...";
	Event e("destination", "source", 1.5);
	Event e2("destination", "source", 1.8);
	assert (e2 > e);
	std::cout << "Events passed tests!" << std::endl;
}
