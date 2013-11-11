//
//  Event.cpp
//  CS 143
//
//  Created by Michael Lauria on 11/10/13.
//  Copyright (c) 2013 Anish. All rights reserved.
//

#include "Event.h"

// Constructor

Event::Event(std::string dest,
             std::string src,
             float ts) {
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
}

float Event::eventTime() const {
    return timestamp;
}

// Override operator to allow eventHeap to order by timestamp
bool Event::operator>(const Event& other) const {
    return timestamp > other.timestamp;
}
