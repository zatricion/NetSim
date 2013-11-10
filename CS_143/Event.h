#include "EventGenerator.h"
#include "Packet.h"
#include <string>

class Event
{
public:
    Event(std::unique_ptr<EventGenerator> dest,
          std::unique_ptr<EventGenerator> src,
          float ts);

    float eventTime() const;
    
    void handleEvent();
    
    std::unique_ptr<EventGenerator> destination;
    std::unique_ptr<EventGenerator> source;

    bool operator>(const Event& other) const;
    
private:

    float timestamp;
};
