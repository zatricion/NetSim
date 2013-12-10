#include "deref_comp.h"

bool operator>(const std::shared_ptr<Event> a, const std::shared_ptr<Event> b)
{
    return a->eventTime() > b->eventTime();
}
