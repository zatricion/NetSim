#include "Path.h"

// Constructor
Path::Path(std::string link) {
    link_vec.push_back(std::make_tuple(link, std::numeric_limits<float>::max(), 0.0));
};

Path::Path(const Path& other) {
    link_vec.clear();
    for (auto& it : other.link_vec) {
        link_vec.push_back(it);
    }
};

void Path::addLink(std::string link, double delay, double ts) {
    link_vec.push_back(std::make_tuple(link, delay, ts));
};

double Path::getTotalDelay() const {
    double total_delay = 0;
    for (auto &it : link_vec)
        total_delay += std::get<1>(it);
    return total_delay;
};

void Path::updateLinkWeight(std::string link, double delay, double time) {
    for (auto &it : link_vec) {
        std::string this_link = std::get<0>(it);
        if ((this_link == link) && (std::get<2>(it) < time)) {
            std::get<1>(it) = delay;
            std::get<2>(it) = time;
            break;
        }
    }
};

void Path::updateAll(const Path& other) {
    for (auto &ot : other.link_vec) {
        updateLinkWeight(std::get<0>(ot), std::get<1>(ot), std::get<2>(ot));
    }
};

bool Path::hasCycle(std::string link) {
    for (auto &it : link_vec) {
        if (std::get<0>(it) == link) {
            return true;
        }
    }
    return false;
};

std::string Path::getNextLink() const {
    return std::get<0>(link_vec.back());
}

std::string Path::to_string() {
    std::string output = "";
    for (auto &it : link_vec) {
        output += "(" + std::to_string(std::get<2>(it)) + ", " + std::get<0>(it) + ", " + std::to_string(std::get<1>(it)) + ")  ";
    }
    return output;
};
    
