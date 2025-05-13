//
//  Transform.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 05.05.25.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include <string>

struct Transform {
    float x;
    float y;
    std::string renderTag;
    
    // Some convenience methods for copying a Transform and only changing a single property
    Transform copyWithX(const float newX) const {
        Transform t = *this;
        t.x = newX;
        return t;
    }
    
    Transform copyWithY(const float newY) const {
        Transform t = *this;
        t.y = newY;
        return t;
    }
};

#endif /* Transform_hpp */
