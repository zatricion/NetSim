#ifndef __NetSim__Path__
#define __NetSim__Path__


#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <tuple>

class Path {

public:
    std::vector<std::tuple<std::string, double, double> > link_vec;
    
    // Constructor
    Path(std::string link = "NONE");
    
    // Copy Constructor
    Path(const Path& other);
    
    // Adds a link to the path
    void addLink(std::string link, double delay, double ts);
    
    // Gets total delay along the path
    double getTotalDelay() const;
    
    // Updates a link weight
    void updateLinkWeight(std::string link, double delay, double time);
    
    // Updates all link weights given another path
    void updateAll(const Path& other);
    
    // Checks a path for a link
    bool hasCycle(std::string link);
    
    // Gets the next link in the path
    std::string getNextLink() const;
    
    // Returns a string representation of a path
    std::string toString();
};


#endif /* defined(__NetSim__Path__) */
