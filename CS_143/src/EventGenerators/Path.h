/**
 * Class to represent a path, used by Routers.
 */


#ifndef __NetSim__Path__
#define __NetSim__Path__


#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <tuple>


class Path {

public:
    // Fields
    /** Vector of links in the path. */
    std::vector<std::tuple<std::string, double, double> > link_vec;
    
    // Constructor
    Path(std::string link = "NONE");
    Path(const Path& other);

    // Accessors
    std::string toString();
    
    // Methods
    void addLink(std::string link, double delay, double ts);
    double getTotalDelay() const;
    void updateLinkWeight(std::string link, double delay, double time);
    void updateAll(const Path& other);
    bool hasCycle(std::string link);
    std::string getNextLink() const;
};


#endif /* defined(__NetSim__Path__) */
