#ifndef CS_143_deref_comp_h
#define CS_143_deref_comp_h

#include "../EventHandling/Event.h"
#include <memory>



bool operator>(const std::shared_ptr<Event> a, const std::shared_ptr<Event> b);

#endif
