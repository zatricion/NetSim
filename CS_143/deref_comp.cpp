#include "deref_comp.h"

bool DerefCompareEvent::operator() (const Event* x, const Event* y)
{
    return x->eventTime() > y->eventTime();
}
