//
//  Renderer.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include "Renderable.hpp"
#include <memory>
#include <vector>
#include "Transform.hpp"
#include "Bounds.hpp"
#include <utility>

class Renderer {
public:
    virtual ~Renderer() = default;
    
    // TODO: create separate method to update the canvasBounds in case they change at runtime (e.g. because the window size changes, for instance)
    virtual void initialize(Bounds canvasBounds) = 0;
    
    /**
     Adds all Renderables to the scene.
     */
    virtual void addToScene(const std::unordered_map<std::string, std::vector<Renderable*>> renderBuckets) = 0;
    virtual void renderFrame(const std::unordered_map<std::string, std::pair<Transform, std::vector<Renderable*>>> renderObjects) = 0;
};

#endif /* Renderer_hpp */
