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
        
    void addLink(std::string link, double delay, double ts);
    
    double getTotalDelay() const;
    
    void updateLinkWeight(std::string link, double delay, double time);
    
    void updateAll(const Path& other);
    
    bool hasCycle(std::string link);
    
    std::string getNextLink() const;
    
    std::string toString();
};


#endif /* defined(__NetSim__Path__) */
