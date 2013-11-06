#include <Event.hh>

bool Event::operator<(const Event &rhs) {
    return this->time < rhs.time;
}

float Event::getTime() {
    return this->time;
}

