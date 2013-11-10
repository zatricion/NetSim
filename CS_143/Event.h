
#include "Packet.h"
#include <string>

class Event
{
public:
<<<<<<< HEAD
    Event(std::unique_ptr<EventGenerator> dest, 
	  std::unique_ptr<EventGenerator> src, 
	  float ts);

=======
    Event(Packet& pkt, std::string, std::string, float);
    
>>>>>>> c5379ad44647621a95ca3ce51f24499e5f716d8e
    float eventTime() const;
    
    void handleEvent();
    
    std::unique_ptr<EventGenerator> destination;
    std::unique_ptr<EventGenerator> source;

    bool operator>(const Event& other) const;
    
private:

    float timestamp;
};
