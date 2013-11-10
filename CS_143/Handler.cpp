#include "Handler.h"
#include <cassert>


float Handler::getMinTime()
{
    assert(generators.size() > 0);
    float minTime = generators.front()->getNextTime();
    // iterate over EventGenerators
    for (std::vector<std::unique_ptr<EventGenerator>>::iterator it = generators.begin();
         it != generators.end();
         it++)
        {
        // update min if smaller
        if ((*it)->getNextTime() < minTime) {
            minTime = (*it)->getNextTime();
        }
    }
    return minTime;
}


// iterate over generators, populate currEvents with imm. events
void Handler::populateCurrentEvents(float minTime) {
    // WARNING: will clear out currEvents...is that desired behavior?
    currEvents.clear();
    for (std::vector<std::unique_ptr<EventGenerator>>::iterator it = generators.begin();
         it != generators.end(); 
         it++) {
        // check to see if current EG has event at desired time
        // add if so
        if ((*it)->getNextTime() == minTime)
            currEvents.push_back((*it)->getEvent());
    }
}

void Handler::processCurrentEvents() {
    for (std::vector<std::unique_ptr<Event>>::iterator it = currEvents.begin();
         it != currEvents.begin();
         it++) {
        // Each event handles itself.
        (*it)->handleEvent();
    }
}

void Handler::step() {
    populateCurrentEvents(getMinTime());
    processCurrentEvents();
}   

