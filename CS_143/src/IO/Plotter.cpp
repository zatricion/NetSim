#include "Plotter.h"
#include "gnuplot-iostream.h"

Plotter sim_plotter = Plotter();

void Plotter::logLinkRate(std::string name,
                          std::tuple<float, float> rate_data) {
    link_rate[name].push_back(rate_data);
    
}

void Plotter::plot(plot_data data) {
    Gnuplot gp;
    for (auto& it : data) {
        std::string title = "plot '-' with lines title '" + it.first + "'\n";
        gp << title;
        gp.send1d(it.second);
    }
}

void Plotter::plotLinkRate() {
    Plotter::plot(link_rate);
}
