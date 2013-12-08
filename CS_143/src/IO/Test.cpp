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
    FILELog::ReportingLevel() = logINFO; //logDEBUG;
    FILE_LOG(logINFO) << "Preparing to test objects.";
    FILE_LOG(logINFO) << "Testing Object constructors.";
    packetTest();
    FILE_LOG(logINFO) << "Testing Simulation.";
    simTest2();
    //simTest1();
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
    auto link1 = std::make_shared<Link>((300 * 8 * 1024.0), // buffer size
                                        0.01, // propagation delay
                                        pow(10, 7), // capacity
                                        "host1", "host2", "link1");
    
    auto host1 = std::make_shared<Host>(link1, "host1");
    auto host2 = std::make_shared<Host>(link1, "host2");
    auto flow1 = std::make_shared<VegasFlow>("flow1", "host2",
                      (30000 * 8 * 1024), host1, 1, 1.0);
    
    std::vector<std::shared_ptr<Flow> > flow_list;
    flow_list.push_back(flow1);
        
    auto flow_g = std::make_shared<FlowGenerator>(flow_list, "flow_g");
    
    FILE_LOG(logDEBUG) << "Adding Network Objects to handler.";
    handler.addGenerator(link1);
    handler.addGenerator(host1);
    handler.addGenerator(host2);
    handler.addGenerator(flow_g);
    
    FILE_LOG(logDEBUG) << "Running simulation.";
    //double runtime = 6.0;
    double runtime = 40.0;

    //while(handler.getMinTime() < runtime)
    while(handler.running())
    {
        handler.step();
    }

    FILE_LOG(logINFO) << "Simulator passed tests!";
    sim_plotter.plotLinkRate(runtime);
    sim_plotter.plotBufferOccupancy(runtime);
    sim_plotter.plotFlowRTT(runtime);
    sim_plotter.plotFlowWindowSize(runtime);
    sim_plotter.plotPacketLoss(runtime);
 
}

/*
void simTest1()
{
    FILE_LOG(logDEBUG) << "Constructing Network Objects.";
    Handler handler = Handler();
    
    // add link1
    auto link1 = std::make_shared<Link>((64 * 64 * 64 * 8 * 1000.0), 0.001, pow(10, 7),
                                        "host1", "router", "link1");

    auto link2 = std::make_shared<Link>((64 * 64 * 64 * 8 * 1000.0), 0.001, pow(10, 7),
                                        "router", "host2", "link2");

    auto ccAlg = std::make_shared<TCPReno>();

    auto host1 = std::make_shared<Host>(link1, "host1");
    auto host2 = std::make_shared<Host>(link2, "host2");

    std::vector<std::string> host_list;
    host_list.push_back(host1->getID());
    host_list.push_back(host2->getID());
 
    auto router = std::make_shared<Router>(host_list, 
       std::vector<std::shared_ptr<Link> > {link1, link2}, "router", 
       std::vector<std::shared_ptr<Link> > {link1, link2});

    auto flow1 = std::make_shared<Flow>("flow1", "host2", ccAlg,
                      (8 * pow(10, 4)), host1, 10, 1.0);

    std::vector<std::shared_ptr<Flow> > flow_list;
    flow_list.push_back(flow1);

    auto flow_g = std::make_shared<FlowGenerator>(flow_list, "flow_g");

    FILE_LOG(logDEBUG) << "Adding Network Objects to handler.";
    handler.addGenerator(link1);
    handler.addGenerator(link2);
    handler.addGenerator(host1);
    handler.addGenerator(host2);
    handler.addGenerator(router);
    handler.addGenerator(flow_g);

    FILE_LOG(logDEBUG) << "Running simulation.";
    while(handler.running())
    {
        handler.step();
    }

    FILE_LOG(logINFO) << "Simulator passed tests!";
}
*/

void simTest2()
{
    FILE_LOG(logDEBUG) << "Constructing Network Objects.";
    Handler handler;
    //handler = read_input();
    
    ////////////
    // add links
    auto link0 = std::make_shared<Link>((64 * 8 * 1024.0), 0.01, 1.25 * pow(10, 7),
                                        "host1", "router1", "link0");
    auto link1 = std::make_shared<Link>((64 * 8 * 1024.0), 0.01, pow(10, 7),
                                        "router1", "router2", "link1");
    auto link2 = std::make_shared<Link>((64 * 8 * 1024.0), 0.01, pow(10, 7),
                                        "router1", "router3", "link2");
    auto link3 = std::make_shared<Link>((64 * 8 * 1024.0), 0.01, pow(10, 7),
                                        "router2", "router4", "link3");
    auto link4 = std::make_shared<Link>((64 * 8 * 1024.0), 0.01, pow(10, 7),
                                        "router3", "router4", "link4");
    auto link5 = std::make_shared<Link>((64 * 8 * 1024.0), 0.01, 1.25 * pow(10, 7),
                                        "router4", "host2", "link5");
    
    // add hosts
    auto host1 = std::make_shared<Host>(link0, "host1");
    auto host2 = std::make_shared<Host>(link5, "host2");
    
    // create host list
    std::vector<std::string> host_list;
    host_list.push_back(host1->getID());
    host_list.push_back(host2->getID());
    
    // create routers
    auto router1 = std::make_shared<Router>(host_list, std::vector<std::shared_ptr<Link> > {link0, link1, link2}, "router1", std::vector<std::shared_ptr<Link> > {link0, link1, link2, link3, link4, link5});
    auto router2 = std::make_shared<Router>(host_list, std::vector<std::shared_ptr<Link> > {link1, link3}, "router2", std::vector<std::shared_ptr<Link> > {link0, link1, link2, link3, link4, link5});
    auto router3 = std::make_shared<Router>(host_list, std::vector<std::shared_ptr<Link> > {link2, link4}, "router3", std::vector<std::shared_ptr<Link> > {link0, link1, link2, link3, link4, link5});
    auto router4 = std::make_shared<Router>(host_list, std::vector<std::shared_ptr<Link> > {link3, link4, link5}, "router4", std::vector<std::shared_ptr<Link> > {link0, link1, link2, link3, link4, link5});
    
    // add flow
    auto flow1 = std::make_shared<VegasFlow>("flow1", "host2",
                                        (20 * 8 * pow(10, 6)), host1, 1, 0.5);
    
    std::vector<std::shared_ptr<Flow> > flow_list;
    flow_list.push_back(flow1);
    
    auto flow_g = std::make_shared<FlowGenerator>(flow_list, "flow_g");
    
    FILE_LOG(logDEBUG) << "Adding Network Objects to handler.";
    handler.addGenerator(link0);
    handler.addGenerator(link1);
    handler.addGenerator(link2);
    handler.addGenerator(link3);
    handler.addGenerator(link4);
    handler.addGenerator(link5);
    
    handler.addGenerator(host1);
    handler.addGenerator(host2);
    
    handler.addGenerator(router1);
    handler.addGenerator(router2);
    handler.addGenerator(router3);
    handler.addGenerator(router4);

    handler.addGenerator(flow_g);
    //////////////
    

    FILE_LOG(logDEBUG) << "Running simulation.";
    double runtime = 50.0;
    while(handler.getMinTime() < runtime)     {
        handler.step();
    }
    
    // output plots
    sim_plotter.plotLinkRate(runtime);
    sim_plotter.plotBufferOccupancy(runtime);
    sim_plotter.plotFlowRTT(runtime);
    sim_plotter.plotFlowWindowSize(runtime);
    sim_plotter.plotPacketLoss(runtime);

    FILE_LOG(logINFO) << "Simulator passed tests!";
}
