#ifndef __NetSim__Plotter__
#define __NetSim__Plotter__

#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <string>
#include <boost/tuple/tuple.hpp>

class Plotter {
    typedef std::unordered_map<std::string, std::vector<std::tuple<double, float> > > plot_data;
    
public:
    void logLinkRate(std::string name,
                     std::tuple<double, float> rate_data);
    
    
    void logBufferOccupancy(std::string link_name,
                     std::tuple<double, float> buffer_data);
    
    

    void logFlowRTT(std::string name,
                     std::tuple<double, float> rate_data);
   

    void logFlowWindowSize(std::string name,
                           std::tuple<double, float> rate_data);
    
    void logPacketLoss(std::string name,
                                std::tuple<double, float> rate_data);
 
    void plot(plot_data data,
              double runtime,
              std::string title,
              std::string xlabel,
              std::string ylabel,
              std::string type = "points");
    
    void plotLinkRate(double);
    void plotBufferOccupancy(double);
    void plotFlowRTT(double);
    void plotFlowWindowSize(double);
    void plotPacketLoss(double);
    
private:
    plot_data linkRate;
    plot_data BufferOccupancy;
    plot_data flowRTT;
    plot_data flowWindowSize;
    plot_data packetLoss;
};

extern Plotter sim_plotter;

#endif /* defined(__NetSim__Plotter__) */
