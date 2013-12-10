/**
 * Abstract class representing a generic Event in our Event based simulation.
 */
#ifndef __CS_143__Event__
#define __CS_143__Event__


#include <string>


class Event {

public:
    // Fields
    std::string destination;
    std::string source;

    // Constructor
    Event(std::string dest, std::string src, double ts);
    
    // Accessors
    double eventTime() const;
    // These are marked as virtual because we want to override them.
    // But they are implemented to give a framework of how they should look.
    virtual std::string toString();
    virtual std::string getType();
    
protected:
    // Fields
    double timestamp;
};


#endif /* defined(__CS_143__Event__) */
