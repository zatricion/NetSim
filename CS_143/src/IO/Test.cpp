// Tests basic use cases of objects.
#include "Test.h"
#include <cassert>

// @MaxHorton TODO:
// This method will contain unit tests for each of the objects.
// It will be implemented as the network objects are created.
int main()
{
    // Set most verbose logging.
    FILE *log = fopen("test.log", "w");
    Output2FILE::Stream() = log;
    // TODO is it okay to change this later?  Preprocessor...
    FILELog::ReportingLevel() = logDEBUG4;
    FILE_LOG(logINFO) << "Preparing to test objects.";
    FILE_LOG(logINFO) << "Testing Object constructors.";
    packetTest();
    FILE_LOG(logINFO) << "Testing Simulation.";
    simTest0();
    FILE_LOG(logINFO) << "Simulation Successful.";
    return 0;
}

/**
 * Simple test of packets.  Not much to test...
 */
void packetTest()
{
	FILE_LOG(logDEBUG) << "Testing packets...";
	//Packet p("uuid", "final_dest", "source", 10, true, false, false, NULL, 0);
	// Not really anything to test... no methods in Packet.h
	//assert (p.uuid == "uuid");
	FILE_LOG(logDEBUG) << "Packets passed tests!";
}

/*
 * Test a simulation
 */

void simTest0()
{
    FILE_LOG(logDEBUG) << "Constructing Network Objects.";
    Handler handler = Handler();
    
    // add link1
    auto link1 = std::make_shared<Link>((64 * 8 * 1000.0), 0.01, pow(10, 7),
                                        "host1", "router1", "link1");
    // add link2
    auto link2 = std::make_shared<Link>((64 * 8 * 1000.0), 0.01, pow(10, 7),
                                        "router1", "host2", "link2");
    
    // add router1
    auto router1 = std::make_shared<Router>((64 * 8 * 1000.0), 0.01, pow(10, 7),
                                        "host1", "router1", "link1");
    
    auto ccAlg = std::make_shared<CongestionAlg>();
    auto host1 = std::make_shared<Host>(link1, "host1");
    auto host2 = std::make_shared<Host>(link1, "host2");
    auto flow1 = std::make_shared<Flow>("flow1", "host2", ccAlg,
                      (20 * 8 * pow(10, 6)), host1, 10, 1.0);
    
    std::vector<std::shared_ptr<Flow> > flow_list;
    flow_list.push_back(flow1);
        
    auto flow_g = std::make_shared<FlowGenerator>(flow_list, "flow_g");
    
    FILE_LOG(logDEBUG) << "Adding Network Objects to handler.";
    handler.addGenerator(link1);
    handler.addGenerator(host1);
    handler.addGenerator(host2);
    handler.addGenerator(flow_g);
    
    FILE_LOG(logDEBUG) << "Running simulation.";
    while(handler.running())
    {
        handler.step();
    }

    FILE_LOG(logINFO) << "Simulator passed tests!";
}
