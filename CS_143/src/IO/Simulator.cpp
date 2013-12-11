#include "Simulator.h"


/**
 * Runs the simulation.
 *
 * @return 0
 */
int main() {
    // Note: this file log was used extensively for debugging.  We have removed
    // Logging statements in most functions to improve runtime.  However, we
    // leave these lines here as an indicator to the reader how logging can
    // be done.  Most files also include the src/Tools/Log.h file already.
    FILE *log = fopen("test.log", "w");
    Output2FILE::Stream() = log;
    FILELog::ReportingLevel() = logINFO;
    FILE_LOG(logINFO) << "Preparing to test objects.";
    FILE_LOG(logINFO) << "Testing Object constructors.";
    
    io();
    return 0;
}


/**
 * Calls the I/O functionality for user input.  Then, parses the input and runs
 * the simulation.
 */
void io() {
    Handler handler;
 
    FILE_LOG(logDEBUG) << "Running simulation.";
    double runtime;
    std::string input;
    std::string f_name;
    
    std::cout << "Enter a time (in seconds) for the simulation to run: ";
    std::ws(std::cin);
    std::getline(std::cin, input);
    std::cout << std::endl;
    
    std::cout << "Enter the name of the file that specifies the test case: ";
    std::ws(std::cin);
    std::getline(std::cin, f_name);
    std::cout << std::endl;
    
    std::ifstream myfile;
    myfile.open(f_name);
    handler = parse(myfile);
    myfile.close();
    
    // hide the cursor
    printf("\e[?25l");
    
    double currTime = handler.getMinTime();
    // set precision so won't flail around too much
    std::cout.precision(3);
    runtime = std::atof(input.c_str());
    while(currTime < runtime && handler.running()) {
        std::cout << "\rTime: " << currTime << " / " << runtime << "             ";
        handler.step();
	currTime = handler.getMinTime();
    }
    std::cout << std::endl;
    
    // show the cursor
    printf("\e[?25h");
    
    // output plots
    sim_plotter.plotLinkRate(runtime);
    sim_plotter.plotBufferOccupancy(runtime);
    sim_plotter.plotFlowRTT(runtime);
    sim_plotter.plotFlowWindowSize(runtime);
    sim_plotter.plotPacketLoss(runtime);
    sim_plotter.plotFlowRate(runtime);

    FILE_LOG(logINFO) << "Simulator passed tests!";
}   
