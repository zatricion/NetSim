#include "deref_comp.h"

bool DerefCompareEvent::operator() (const std::shared_ptr<Event> x, const std::shared_ptr<Event> y)
{
    return x->eventTime() > y->eventTime();
}
