#include "Path.h"

// Constructor
Path::Path(std::string link) {
    link_vec.push_back(std::tuple<std::string, double>(link, std::numeric_limits<float>::max()));
};

Path::Path(const Path& other) {
    link_vec.clear();
    for (auto& it : other.link_vec) {
        link_vec.push_back(it);
    }
};

void Path::addLink(std::string link, double delay) {
    link_vec.push_back(std::tuple<std::string, double>(link, delay));
};

double Path::getTotalDelay() const {
    double total_delay = 0;
    for (auto &it : link_vec)
        total_delay += std::get<1>(it);
    return total_delay;
};

void Path::updateLinkWeight(std::string link, double delay) {
    for (auto &it : link_vec) {
        if (std::get<0>(it) == link) {
            std::get<1>(it) = delay;
            break;
        }
    }
};

void Path::updateAll(const Path& other) {
    for (auto &it : link_vec) {
        for (auto &ot : other.link_vec) {
            if (std::get<0>(it) == std::get<0>(ot)) {
                std::get<1>(it) = std::get<1>(ot);
            }
        }
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
        output += "(" + std::get<0>(it) + ", " + std::to_string(std::get<1>(it)) + ")  ";
    }
    return output;
};
    
