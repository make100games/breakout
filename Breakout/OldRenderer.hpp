//
//  Renderer.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 28.04.25.
//

#ifndef OldRenderer_hpp
#define OldRenderer_hpp

#include <stdio.h>

class OldRenderer {
public:
    OldRenderer();
    ~OldRenderer();
    
    void initialize();
    void renderFrame();
    
private:
    // Vertex arrays (VAOs) and vertex buffers (VBOs)
    // TODO: for breakout I will need 3 pairs of VAOs and VBOs:
    // 1 pair for the paddle
    // 1 pair for the ball
    // 1 pair for all of the bricks -> only draw vertices in buffer of bricks that have not been destroyed yet
    // The reason why we want 3 pairs is because each of these 3 objects (paddle, ball and brick wall) need to be
    // able to move independently and they have a different shape.
    unsigned int VAO1;
    unsigned int VBO1;
    unsigned int VAO2;
    unsigned int VBO2;
    unsigned int shaderProgram;

    void setupShader();
    void setupGeometry();
};

#endif /* OldRenderer_hpp */
