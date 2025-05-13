//
//  OpenGlRenderer.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 03.05.25.
//

#ifndef OpenGlRenderer_hpp
#define OpenGlRenderer_hpp

#include <stdio.h>
#include "Renderer.hpp"
#include "Renderable.hpp"
#include <memory>
#include <vector>
#include "Transform.hpp"
#include "Bounds.hpp"
#include <unordered_map>
#include <utility>

struct RenderingContext {
    // TODO: change these from unsigned int to GLuint
    unsigned int VAO;   // Vertex array object
    unsigned int VBO;   // Vertex buffer object
};

class OpenGlRenderer : public Renderer {
public:
    OpenGlRenderer();
    ~OpenGlRenderer();
    
    void initialize(Bounds canvasBounds) override;
    void addToScene(const std::unordered_map<std::string, std::vector<Renderable*>> renderBuckets) override;
    void renderFrame(const std::unordered_map<std::string, std::pair<Transform, std::vector<Renderable*>>> renderObjects) override;
private:
    // The key of the hashmap is the rendering tag of a Renderable or Transform (those 2 renderingTags MUST BE IDENTICAL! I'm not super happy with this design but it's ok as a first step.
    std::unordered_map<std::string, RenderingContext> renderingContexts;
    unsigned int VAO1;
    unsigned int VBO1;
    unsigned int shaderProgram;
    
    void setupShader();
    void setupGeometry(std::string renderTag, std::vector<Renderable*> renderables);
};

#endif /* OpenGlRenderer_hpp */
