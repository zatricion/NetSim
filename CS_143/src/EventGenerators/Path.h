//
//  Path.h
//  NetSim
//
//  Created by Michael Lauria on 11/28/13.
//  Copyright (c) 2013 Michael Lauria. All rights reserved.
//

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
    Path(std::string link) {
        link_vec.push_back(std::tuple<std::string, float>(link, std::numeric_limits<float>::max()));
    };
    
    Path(const Path& other) {
        link_vec.clear();
        for (auto& it : other.link_vec) {
            link_vec.push_back(it);
        }
    };
    
    //Path()~
    
    void push_back(std::string link, float delay) {
        link_vec.push_back(std::tuple<std::string, float>(link, delay));
    };
    
    float getTotalDelay() {
        float total_delay = 0;
        for (auto &it : link_vec)
            total_delay += std::get<1>(it);
        return total_delay;
    };
    
    void updateLinkWeight(std::string link, float delay) {
        for (auto &it : link_vec) {
            if (std::get<0>(it) == link) {
                it = std::tuple<std::string, float>(link, delay);
                break;
            }
        }
    };
    
    bool hasCycle(std::string link) {
        for (auto &it : link_vec) {
            if (std::get<0>(it) == link) {
                return true;
            }
        }
        return false;
    };
    
    std::string to_string() {
        std::string output = "";
        for (auto &it : link_vec) {
            output += "(" + std::get<0>(it) + ", " + std::to_string(std::get<1>(it)) + ")  ";
        }
        return outputs;
    };
    
};


#endif /* defined(__NetSim__Path__) */
