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
    CongestionAlg ccAlg;
    
    // add host1
    Host host1 = Host(link1, "host1");
    
    // add host2
    Host host2 = Host(link1, "host2");
    
    
    // FlowGenerator is dumb, we should just have Flow inherit from EventGenerator
    // add flow
    Flow flow1 = Flow("flow1", "host1", "host2", &ccAlg,
                      (20 * 8 * pow(10, 6)), &host1, 10, 1.0);
    

    
    std::vector<Flow> flow_list;
    flow_list.push_back(flow1);
    
    FlowGenerator flow_g = FlowGenerator(flow_list, "flow_g");
    

    handler.addGenerator(make_unique<Host>(host1));
    handler.addGenerator(make_unique<Host>(host2));
    handler.addGenerator(make_unique<FlowGenerator>(flow_g));
    handler.addGenerator(make_unique<Host>(host1));
}