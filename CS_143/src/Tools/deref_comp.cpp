#include "deref_comp.h"

//bool DerefCompareEvent::operator() (const std::shared_ptr<Event> x, const std::shared_ptr<Event> y)
//{
//    return x->eventTime() > y->eventTime();
//}


//bool operator<(const shared_ptr<Int> a, const shared_ptr<Int> b)
//{
//    return a->getVal() < b->getVal();
//}

bool operator>(const std::shared_ptr<Event> a, const std::shared_ptr<Event> b)
{
    return a->eventTime() > b->eventTime();
}
