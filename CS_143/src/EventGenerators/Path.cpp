#include "Path.h"


/**
 * Constructor for instance of a Path.
 *
 * @param link the name of the link used to initialize the path.
 */
Path::Path(std::string link) {
    link_vec.push_back(std::make_tuple(link, std::numeric_limits<float>::max(), 0.0));
};


/**
 * Copy constructor for Path.
 *
 * @param other the path to copy.
 */
Path::Path(const Path& other) {
    link_vec.clear();
    for (auto& it : other.link_vec) {
        link_vec.push_back(it);
    }
};


/**
 * Adds a link to the path.
 *
 * @param link the name of the link to add.
 * @param delay the delay of the link.
 * @param ts the time the link delay was most recently updated.
 */
void Path::addLink(std::string link, double delay, double ts) {
    link_vec.push_back(std::make_tuple(link, delay, ts));
};


/**
 * Gets the total delay along a Path.
 *
 * @return the total delay.
 */
double Path::getTotalDelay() const {
    double total_delay = 0;
    for (auto &it : link_vec)
        total_delay += std::get<1>(it);
    return total_delay;
};


/**
 * Updates a link weight in a Path. Only updates if the link delay is less current
 * than the new delay.
 *
 * @param link the name of the link to update.
 * @param delay the new delay for the link.
 * @param time the time for which the new delay is current.
 */
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


/**
 * Updates all link weights in a Path.
 *
 * @param other the Path providing update information.
 */
void Path::updateAll(const Path& other) {
    for (auto &ot : other.link_vec) {
        updateLinkWeight(std::get<0>(ot), std::get<1>(ot), std::get<2>(ot));
    }
};


/**
 * Determines whether a path includes a given link.
 *
 * @param link the link it is looking for in the path.
 * @return whether the path includes the link given.
 */
bool Path::hasCycle(std::string link) {
    for (auto &it : link_vec) {
        if (std::get<0>(it) == link) {
            return true;
        }
    }
    return false;
};


/**
 * Gets the next link in a Path.
 *
 * @return the next link.
 */
std::string Path::getNextLink() const {
    return std::get<0>(link_vec.back());
}


/**
 * Represents the Path as a string.
 *
 * @return the string representing the path.
 */
std::string Path::toString() {
    std::string output = "";
    for (auto &it : link_vec) {
        output += "(" + std::to_string(std::get<2>(it)) + ", " + std::get<0>(it) + ", " + std::to_string(std::get<1>(it)) + ")  ";
    }
    return output;
};
