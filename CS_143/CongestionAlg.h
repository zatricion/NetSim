#ifndef __CS_143__CongestionAlg__
#define __CS_143__CongestionAlg__

#include <iostream>
#include "Flow.h"

class CongestionAlg
{
public:
    // TODO these names suck.
    void call(Flow *flow);
    void call(Flow *flow, UnackEvent e);
    void call(Flow *flow, PacketEvent e);
};

#endif /* defined(__CS_143__CongestAlg__) */
