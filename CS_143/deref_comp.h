#ifndef CS_143_deref_comp_h
#define CS_143_deref_comp_h

#include "Event.h"

class DerefCompareEvent
{
public:
    bool operator() (const Event* x, const Event* y);
};


#endif