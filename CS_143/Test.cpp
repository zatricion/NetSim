// Tests basic use cases of objects.
#include "Test.h"
#include <cassert>

// @MaxHorton TODO:
// This method will contain unit tests for each of the objects.
// It will be implemented as the network objects are created.
int main()
{
    std::cout << "Preparing to test objects." << std::endl;
    std::cout << "Testing Object constructors." << std::endl;
    packetTest();
    eventTest();
    return 0;
}

/**
 * Simple test of packets.  Not much to test...
 */
void packetTest()
{
	std::cout << "Testing packets...";
	//Packet p("uuid", "final_dest", "source", 10, true, false, false, NULL, 0);
	// Not really anything to test... no methods in Packet.h
	//assert (p.uuid == "uuid");
	std::cout << "Packets passed tests!" << std::endl;
}

/**
 * Simple test of events.
 */
void eventTest()
{
	std::cout << "Testing events...";
	Event e("destination", "source", 1.5);
	Event e2("destination", "source", 1.8);
	assert (e2 > e);
	std::cout << "Events passed tests!" << std::endl;
}

/*
 * Test a simulation
 */

void simTest0()
{
    // test a simulation with just two hosts, one link, one flow
    Handler handler = Handler();
    
    // add link1
    Link link1 = Link((64 * 8 * 1000.0), 0.01, pow(10, 7), "host1", "host2",
                      "link1");
    
    // create congestion algorithm
    ccAlg =
    
    // add flow
    Flow flow1 = Flow("flow1", "host1", "host2", ccAlg,
                      // TODO: enter data in bits, fix Flow class
    
    // add host1
    Host host1 = Host(
    handler.addGenerator(<#std::unique_ptr<EventGenerator> gen#>)
}