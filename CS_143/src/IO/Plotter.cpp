#include "Plotter.h"
#include "gnuplot-iostream.h"


// For get calls
#include <tuple>
#include <iostream>
#include <string>

Plotter sim_plotter = Plotter();

void Plotter::logLinkRate(std::string name,
                          std::tuple<float, float> rate_data) {
    link_rate[name].push_back(rate_data);
    
}

void Plotter::logBufferOccupancy(std::string name,
                          std::tuple<float, float> rate_data) {
    BufferOccupancy[name].push_back(rate_data);
    
}

void Plotter::logFlowRTT(std::string name,
                         std::tuple<float, float> rate_data) {
    flowRTT[name].push_back(rate_data);
}

void Plotter::plot(plot_data data) {
    Gnuplot gp;
    
//    gp << "set xrange [0:5]\nset yrange [0:10]\n";

    gp << "set xrange [0:3]\nset yrange [0:1000000000]\n";

    std::string cmd = "plot ";

    for (auto& it : data) {
        cmd +=  "'-' with points title '" + it.first + "', ";
    }
    
    // pop off last two character (i.e. ', ') or doesn't seem to work
    cmd.erase(cmd.begin() + cmd.size() - 2);
    cmd += "\n";
    gp << cmd;

    for (auto& it : data) {
        
        //std::cout << std::get<0>(it.second.front()) << " " << std::get<1>(it.second.front()) << std::endl;
       
        gp.send1d(it.second);
    }
}

void Plotter::plotLinkRate() {
    Plotter::plot(link_rate);
}

void Plotter::plotBufferOccupancy() {
    Plotter::plot(BufferOccupancy);
}

void Plotter::plotFlowRTT() {
    Plotter::plot(flowRTT);
}
