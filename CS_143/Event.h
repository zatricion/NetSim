
#include "Packet.h"
#include <string>

class Event
{
public:
    Event(Packet &pkt, std::string, std::string, float);
    
    float eventTime() const;
    
    void handleEvent();
    
    Packet packet;
    std::string destination;
    std::string source;

    bool operator>(const Event& other) const;
    
private:
    float timestamp;
};

