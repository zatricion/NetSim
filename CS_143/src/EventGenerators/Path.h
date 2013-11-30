#ifndef __NetSim__Path__
#define __NetSim__Path__


#include <iostream>
#include <string>
#include <vector>
#include <limits>

/*
#include <unordered_map>
#include <algorithm>
#include "../EventHandling/Packet.h"
#include "../EventHandling/PacketEvent.h"
#include "Host.h"
*/


class Path {

    
public:
    std::vector<std::tuple<std::string, float> > link_vec;
    
    // Constructor
    Path(std::string link = "NONE");
    
    // Copy Constructor
    Path(const Path& other);
    
    //Path()~
    
    void addLink(std::string link, float delay);
    
    float getTotalDelay() const;
    
    void updateLinkWeight(std::string link, float delay);
    
    void updateAll(const Path& other);
    
    bool hasCycle(std::string link);
    
    std::string getNextLink() const;
    
    std::string to_string();
};


#endif /* defined(__NetSim__Path__) */
