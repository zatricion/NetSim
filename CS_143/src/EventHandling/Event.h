#ifndef __CS_143__Event__
#define __CS_143__Event__

#include <string>

class Event
{
public:
    Event(std::string dest, std::string src, double ts);
    
    double eventTime() const;
        
    std::string destination;
    std::string source;
    
    virtual std::string toString();
    virtual std::string getType();
    
protected:
    double timestamp;
};

#endif /* defined(__CS_143__Event__) */
