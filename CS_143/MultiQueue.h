#ifndef CS_143_MultiQueue_h
#define CS_143_MultiQueue_h

// multiple queue class TODO: see if it actually works
template<typename T> class MultiQueue
{
public:
    void addQueue(std::queue<T> q)
    {
        mQ.push_back(q);
    };
    
    void deleteQueue()
    {
        mQ.erase(mQ.begin() + index);
    };
    
    T pop()
    {
        assert (!mQ.empty());
        if (mQ[index].empty())
        {
            deleteQueue();
            return this->pop();
        }
        T res = mQ[index].front();
        mQ[index].pop();
        index = (index + 1) % mQ.size;
        return res;
    }
    
private:
    std::vector<std::queue<T>> mQ;
    int index;
};

#endif
