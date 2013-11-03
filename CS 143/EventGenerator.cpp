//
//  File.cpp
//  CS 143
//
//  Created by Anish on 11/2/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#include "EventGenerator.h"
#include "Event.h"
#include <queue>

class EventGen
{
protected:
    std::priority_queue<int, std::vector<int>, std::greater<int> > eventHeap;
public:
    virtual ~EventGen();
    
    std::string getId() const;
    virtual
    virtual void addEvent() = 0;
    virtual Event getEvent() = 0;
    
    virtual void Method4() = 0;
};
