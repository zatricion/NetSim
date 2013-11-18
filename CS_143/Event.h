#ifndef __CS_143__Event__
#define __CS_143__Event__

#include <string>

class Event
{
public:
    Event(std::string dest, std::string src, float ts);
    
    float eventTime() const;
        
    std::string destination;
    std::string source;
    
    virtual void printEvent();
    virtual std::string getType();
    
protected:
    float timestamp;
};

#endif /* defined(__CS_143__Event__) */
