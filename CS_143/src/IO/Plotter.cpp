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

void Plotter::plot(plot_data data) {
    Gnuplot gp;
    
    gp << "set xrange [0:50]\nset yrange [0:10]\n";

    std::string cmd = "plot ";

    for (auto& it : data) {
        cmd +=  "'-' with points title '" + it.first + "', ";
    }
    cmd += "\n";
    gp << cmd;

    for (auto& it : data) {
        
//        std::cout << std::get<0>(it.second.front()) << " " << std::get<1>(it.second.front()) << std::endl;
       
        gp.send1d(it.second);
    }
}

void Plotter::plotLinkRate() {
    Plotter::plot(link_rate);
}
