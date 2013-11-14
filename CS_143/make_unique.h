#ifndef CS_143_make_shared_h
#define CS_143_make_shared_h

#include <memory>

template<typename T, typename... Args>
std::shared_ptr<T> make_shared(Args&&... args)
{
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif
