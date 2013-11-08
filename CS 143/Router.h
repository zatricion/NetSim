//
//  Router.h
//  CS 143
//
//  Created by Michael Lauria on 11/7/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#ifndef __CS_143__Router__
#define __CS_143__Router__

#include <string>

class Router : public Device
{
public:
    // Routing table maps destination host ids to link idsomm
    std::map<std::string, std::string> routing_table;
};


#endif /* defined(__CS_143__Router__) */
