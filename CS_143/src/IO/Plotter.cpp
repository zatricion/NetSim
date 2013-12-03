#include "Plotter.h"
#include "gnuplot-iostream.h"


// For get calls
#include <tuple>
#include <iostream>
#include <string>

Plotter sim_plotter = Plotter();

void Plotter::logLinkRate(std::string name,
                          std::tuple<float, float> rate_data) {
    linkRate[name].push_back(rate_data);
    
}

void Plotter::logBufferOccupancy(std::string name,
                          std::tuple<float, float> rate_data) {
    BufferOccupancy[name].push_back(rate_data);
    
}

void Plotter::logFlowRTT(std::string name,
                         std::tuple<float, float> rate_data) {
    flowRTT[name].push_back(rate_data);
}

void Plotter::logFlowWindowSize(std::string name,
                         std::tuple<float, float> rate_data) {
    flowWindowSize[name].push_back(rate_data);
}

void Plotter::logPacketLoss(std::string name,
                             std::tuple<float, float> rate_data) {
    packetLoss[name].push_back(rate_data);
}

void Plotter::logPacketDelay(std::string name,
                            std::tuple<float, float> rate_data) {
    packetDelay[name].push_back(rate_data);
}

void Plotter::logFlowRate(std::string name,
                            std::tuple<float, float> rate_data) {
    flowRate[name].push_back(rate_data);
}

void Plotter::plot(plot_data data,
                   float runtime,
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
        gp.send1d(it.second);
    }
}

void Plotter::plotLinkRate(float runtime) {
    Plotter::plot(linkRate,
                  runtime,
                  "Link Rate", "Time (s)", "Link Rate (bps)");
}

void Plotter::plotBufferOccupancy(float runtime) {
    Plotter::plot(BufferOccupancy,
                  runtime,
                  "Buffer Occupancy", "Time (s)", "Queue Size (bits)", "lines");
}

void Plotter::plotFlowRTT(float runtime) {
    Plotter::plot(flowRTT,
                  runtime,
                  "Flow RTT", "Time (s)", "RTT (s)");
}

void Plotter::plotFlowWindowSize(float runtime) {
    Plotter::plot(flowWindowSize,
                  runtime,
                  "Window Size", "Time (s)", "Window Size (pkts)");
}

void Plotter::plotPacketLoss(float runtime) {
    Plotter::plot(packetLoss,
                  runtime,
                  "Packet Loss", "Time (s)", "Number of Packets", "lines");
}

void Plotter::plotPacketDelay(float runtime) {
    Plotter::plot(packetDelay,
                  runtime,
                  "Packet Delay", "Time (s)", "Packet Delay (s)", "lines");
}
void Plotter::plotFlowRate(float runtime) {
    Plotter::plot(flowRate,
                  runtime,
                  "Flow Rate", "Time (s)", "Flow Rate (bps)", "lines");
}
