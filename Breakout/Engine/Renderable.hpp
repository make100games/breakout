//
//  Renderable.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 03.05.25.
//

#ifndef Renderable_hpp
#define Renderable_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Vec3.hpp"

class Renderable {
public:
    virtual ~Renderable() = default;
    
    /**
     The vertices that make up the geometry that is to be rendered. Returning a reference because we don't want to copy potentially
     large numbers of vertices each time this method is called.
     */
    virtual const std::vector<float>& vertices() = 0;
    
    /**
     The color of the renderable.
     */
    virtual const Vec3& color() const = 0;
    
    /**
     A tag that indicates in what context this renderable should be rendered. Renderables with the same tag will be rendered together, translated
     together etc. As a rule of thumb: each renderable that can move independently of others should have its own renderTag.
     */
    virtual std::string renderTag() = 0;
    
    // TODO: return the color as well
};

#endif /* Renderable_hpp */
