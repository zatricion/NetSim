#ifndef __NetSim__Plotter__
#define __NetSim__Plotter__

#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <string>
#include <boost/tuple/tuple.hpp>

class Plotter {
    typedef std::unordered_map<std::string, std::vector<std::tuple<float, float> > > plot_data;
    
public:
    void logLinkRate(std::string name,
                     std::tuple<float, float> rate_data);
    
    void plotLinkRate();
    
    void logBufferOccupancy(std::string link_name,
                     std::tuple<float, float> buffer_data);
    
    void plotBufferOccupancy();
    
    void plot(plot_data data);
    
private:
    plot_data link_rate;
    plot_data BufferOccupancy;
    
};

extern Plotter sim_plotter;

#endif /* defined(__NetSim__Plotter__) */
