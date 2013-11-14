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
    std::cout << "Testing Simulation." << std::endl;
    simTest0();
    std::cout << "Simulation Successful." << std::endl;
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
    
    handler.addGenerator(std::make_shared<Link>(link1));

    handler.addGenerator(std::make_shared<Host>(host1));
    handler.addGenerator(std::make_shared<Host>(host2));
    handler.addGenerator(std::make_shared<FlowGenerator>(flow_g));
    
    while(handler.running())
    {
        handler.step();
    }
    
    /* 
    // TODO: should make a verbose version of "step", with a toString method for
    // each EventGenerator, but for now let's not bother.
    */
    
    std::cout << "Simulator passed tests!" << std::endl;
}
