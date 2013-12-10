#include "Plotter.h"
#include "gnuplot-iostream.h"


// For get calls
#include <tuple>
#include <iostream>
#include <string>

Plotter sim_plotter = Plotter();

void Plotter::logLinkRate(std::string name,
                          std::tuple<double, float> rate_data) {
    linkRate[name].push_back(rate_data);
    
}

void Plotter::logBufferOccupancy(std::string name,
                          std::tuple<double, float> rate_data) {
    BufferOccupancy[name].push_back(rate_data);
    
}

void Plotter::logFlowRTT(std::string name,
                         std::tuple<double, float> rate_data) {
    flowRTT[name].push_back(rate_data);
}

void Plotter::logFlowWindowSize(std::string name,
                         std::tuple<double, float> rate_data) {
    flowWindowSize[name].push_back(rate_data);
}

void Plotter::logPacketLoss(std::string name,
                             std::tuple<double, float> rate_data) {
    packetLoss[name].push_back(rate_data);
}

void Plotter::plot(plot_data data,
                   double runtime,
                   std::string title,
                   std::string xlabel,
                   std::string ylabel,
                   std::string type) {
    Gnuplot gp;
    
    std::string range = "set xrange [0:" + std::to_string(runtime) + "]\n";
    
    std::string t = "set title '" + title + "'\n";
    std::string x = "set xlabel '" + xlabel + "'\n";
    std::string y = "set ylabel '" + ylabel + "'\n";
    
    gp << range;
    
    gp << t;
    gp << x;
    gp << y;
    
    std::string cmd = "plot ";

    for (auto& it : data) {
        cmd +=  "'-' with " + type + " title '" + it.first + "', ";
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

void Plotter::plotLinkRate(double runtime) {
    Plotter::plot(linkRate,
                  runtime,
                  "Link Rate", "Time (s)", "Link Rate (bps)", "lines");
}

void Plotter::plotBufferOccupancy(double runtime) {
    Plotter::plot(BufferOccupancy,
                  runtime,
                  "Buffer Occupancy", "Time (s)", "Queue Size (bits)", "lines");
}

void Plotter::plotFlowRTT(double runtime) {
    Plotter::plot(flowRTT,
                  runtime,
                  "Flow RTT", "Time (s)", "RTT (s)", "lines");
}

void Plotter::plotFlowWindowSize(double runtime) {
    Plotter::plot(flowWindowSize,
                  runtime,
                  "Window Size", "Time (s)", "Window Size (pkts)", "lines");
}

void Plotter::plotPacketLoss(double runtime) {
    Plotter::plot(packetLoss,
                  runtime,
                  "Packet Loss", "Time (s)", "Number of Packets", "lines");
}
