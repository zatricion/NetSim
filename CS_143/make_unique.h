//
//  make_unique.h
//  CS_143
//
//  Created by Michael Lauria on 11/10/13.
//  Copyright (c) 2013 Michael Lauria. All rights reserved.
//

#ifndef CS_143_make_unique_h
#define CS_143_make_unique_h

#include <memory>

template <typename Value, typename ... Arguments>
std::unique_ptr<Value> make_unique(Arguments && ... arguments_for_constructor)
{
    return std::unique_ptr<Value>(new Value(std::forward<Arguments>(arguments_for_constructor)...));
}

#endif
