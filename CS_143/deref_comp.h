#ifndef CS_143_deref_comp_h
#define CS_143_deref_comp_h

class Event;

#include <functional>
#include <iostream>
#include <string>
#include "Event.h"

struct DereferenceCompareEvent : public std::binary_function<Event*, Event*, bool>
{
    bool operator()(const Event* lhs, const Event* rhs) const
    {
        return lhs->eventTime() > rhs->eventTime();
    }
};

#endif
