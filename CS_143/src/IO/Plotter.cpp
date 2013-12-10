#include "Plotter.h"
#include "gnuplot-iostream.h"


// For get calls
#include <tuple>
#include <iostream>
#include <string>

Plotter sim_plotter = Plotter();

/**
 * Log the link rate.
 * 
 * @param name the name of the link
 * @param rate_data the data to log
 */
void Plotter::logLinkRate(std::string name,
                          std::tuple<double, double> rate_data) {
    link_rate[name].push_back(rate_data);
    
}

/**
 * Log the buffer occupancy.
 * 
 * @param name the name of the the thing being logged
 * @param rate_data the data to log
 */
void Plotter::logBufferOccupancy(std::string name,
                          std::tuple<double, double> rate_data) {
    buffer_occupancy[name].push_back(rate_data);
    
}

/**
 * Log the link flow round-trip time.
 * 
 * @param name the name of the the thing being logged
 * @param rate_data the data to log
 */
void Plotter::logFlowRTT(std::string name,
                         std::tuple<double, double> rate_data) {
    flow_RTT[name].push_back(rate_data);
}

/**
 * Log the flow rate.
 * 
 * @param name the name of the the thing being logged
 * @param rate_data the data to log
 */
void Plotter::logFlowRate(std::string name,
                         std::tuple<double, double> rate_data) {
    flow_rate[name].push_back(rate_data);
}

/**
 * Log the flow window size.
 * 
 * @param name the name of the the thing being logged
 * @param rate_data the data to log
 */
void Plotter::logFlowWindowSize(std::string name,
                         std::tuple<double, double> rate_data) {
    flow_window_size[name].push_back(rate_data);
}

/**
 * Log the packet loss.
 * 
 * @param name the name of the thing being logged
 * @param rate_data the data to log
 */
void Plotter::logPacketLoss(std::string name,
                             std::tuple<double, double> rate_data) {
    packet_loss[name].push_back(rate_data);
}

/**
 * Run the plotting code on the accumulated data.
 *
 * @param data the data to be plotted.
 * @param runtime the amount of time to plot for.
 * @param title the title of the plot
 * @param xlabel the x-axis label
 * @param ylabel the y-axis label
 * @param type the type of things to plot with - lines, points, etc. Defaults
 *             to points.
 */
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

/**
 * Plot the link rate.
 *
 * @param runtime the time to plot for.
 */
void Plotter::plotLinkRate(double runtime) {
    Plotter::plot(link_rate,
                  runtime,
                  "Link Rate", "Time (s)", "Link Rate (bps)", "lines");
}

/**
 * Plot the buffer occupancy.
 *
 * @param runtime the time to plot for.
 */
void Plotter::plotBufferOccupancy(double runtime) {
    Plotter::plot(buffer_occupancy,
                  runtime,
                  "Buffer Occupancy", "Time (s)", "Queue Size (bits)", "lines");
}

/**
 * Plot the flow round-trip time.
 *
 * @param runtime the time to plot for.
 */
void Plotter::plotFlowRTT(double runtime) {
    Plotter::plot(flow_RTT,
                  runtime,
                  "Flow RTT", "Time (s)", "RTT (s)", "lines");
}

/**
 * Plot the flow rate.
 *
 * @param runtime the time to plot for.
 */
void Plotter::plotFlowRate(double runtime) {
    Plotter::plot(flow_rate,
                  runtime,
                  "Flow Rate", "Time (s)", "Rate (bps)", "lines");
}

/**
 * Plot the flow window size.
 *
 * @param runtime the time to plot for.
 */
void Plotter::plotFlowWindowSize(double runtime) {
    Plotter::plot(flow_window_size,
                  runtime,
                  "Window Size", "Time (s)", "Window Size (pkts)", "lines");
}

/**
 * Plot the packet loss.
 *
 * @param runtime the time to plot for.
 */
void Plotter::plotPacketLoss(double runtime) {
    Plotter::plot(packet_loss,
                  runtime,
                  "Packet Loss", "Time (s)", "Number of Packets", "lines");
}
