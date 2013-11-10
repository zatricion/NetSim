#include "Host.h"

Host::Host(CongestionAlg control, std::string uuid)
{
    
}

Packet Host::createPacket(<#std::string#> uuid)
class Host : public Device
{
public:
    int window_size;
    
    // Constructor takes a congestion control algorithm and an id string
    Host(CongestionAlg, std::string);
    
    Packet createPacket(std::string);
};