//
//  Rectangle.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 03.05.25.
//

#ifndef Rectangle_hpp
#define Rectangle_hpp

#include <stdio.h>
#include "Renderable.hpp"
#include <string>
#include <vector>
#include "Vec3.hpp"

class Rectangle : public Renderable {
public:
    Rectangle(std::vector<float> vertices, Vec3 color, std::string renderTag);
    ~Rectangle();
    
    const std::vector<float>& vertices() override;
    
    const Vec3& color() const override;
    
    std::string renderTag() override;
    
private:
    std::vector<float> myVertices;
    Vec3 myColor;
    std::string myRenderTag;
};

#endif /* Rectangle_hpp */
