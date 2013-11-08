
class Event {

public:
    /*  
    - PacketPassEvent
    - PacketDropEvent
    */
    static float getTime();
    bool operator<(const Event &rhs);
    virtual void handleEvent();

private:
    float time;
};
