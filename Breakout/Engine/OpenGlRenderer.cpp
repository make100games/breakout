//
//  OpenGlRenderer.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 03.05.25.
//

#define GL_SILENCE_DEPRECATION
#include "OpenGlRenderer.hpp"
#include <OpenGL/gl3.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include "math_utils.hpp"

static const char* vertextShaderSource = R"(
#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 uProjection;
uniform vec2 uOffset;

void main() {
    vec4 worldPosition = vec4(aPos.xy + uOffset, 0.0, 1.0);
    gl_Position = uProjection * worldPosition;
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core

uniform vec3 uColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

void OpenGlRenderer::setupShader() {
    // Compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertextShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Compilation failed: \n" << infoLog << std::endl;
    }
    
    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Failed:\n" << infoLog << std::endl;
    }
    
    // Link shaders into a program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindAttribLocation(shaderProgram, 0, "position"); // Bind 'position' to location 0
    glLinkProgram(shaderProgram);
    
    // Check linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed:\n" << infoLog << std::endl;
    }
    
    // Delete shaders as they are now linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGlRenderer::setupGeometry(std::string renderTag, std::vector<Renderable*> renderables) {
    RenderingContext context = renderingContexts[renderTag];
    
    // Collect the vertices of all Renderables in this list of Renderables
    std::vector<float> allVertices;
    for (auto& renderable : renderables) {
        std::vector<float> renderableVertices = renderable -> vertices();
        allVertices.insert(allVertices.end(), renderableVertices.begin(), renderableVertices.end());
    }
    float* vertices = allVertices.data();
    
    // Generate and bind VBOs and VAOs and upload the vertices to the GPU and such
    // *********** VBO1 and VAO1 ***********
    glGenVertexArrays(1, &context.VAO);
    glGenBuffers(1, &context.VBO);
    glBindVertexArray(context.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, context.VBO);
    
    // This call here actually copies the vertices declared above to the GPUs memory. This makes use of the
    // currently bound VBO and VAO hence you don't need to specify them explicitly here. Yay for OpenGL being
    // a state machine. Not...
    // Ok a quick note here. I had to use renderableVertices.size() to make this work because calling
    // sizeof(vertices) gives you the size of the pointer itself and not the size of the array it's pointing to. Calling sizeof(vertices) where vertices is a float[] will give you the correct size. Just a fun little bug I ran into. Yay. So fun. Much wow.
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), vertices, GL_STATIC_DRAW);
    
    // Define how the shader should read the vertex data. This, again, considers the currently bound VAO and VBO
    glVertexAttribPointer(
        0,                  // Attribute location in shader (layout(location = 0))
        2,                  // Number of components (e.g. vec2 → 2)
        GL_FLOAT,           // Type
        GL_FALSE,           // Normalize?
        2 * sizeof(float),  // Stride (distance between vertices). 2 because a vertex has 2 floats (x and y component)
        (void*)0            // Offset to first component
    );
    glEnableVertexAttribArray(0);   // Atribute 0 = 'position'
    
    // Reset things to ensure a clean state. OpenGL is a state machine. Make
    // sure we reset the state so future operations don't start with wrong
    // assumptions
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    renderingContexts[renderTag] = context;
}

OpenGlRenderer::OpenGlRenderer(): VAO1(0), VBO1(0), shaderProgram(0) {
    
}

OpenGlRenderer::~OpenGlRenderer() {
    if (shaderProgram) glDeleteProgram(shaderProgram);
    for(const auto& pair : renderingContexts) {
        if(pair.second.VBO) glDeleteBuffers(1, &pair.second.VBO);
        if(pair.second.VAO) glDeleteVertexArrays(1, &pair.second.VAO);
    }
    renderingContexts.clear();
}

void OpenGlRenderer::initialize(Bounds canvasBounds) {
    setupShader();
    glClearColor(0.2f, 0.4f, 0.6f, 1.0f); // Background color
    
    // Calculate projection matrix from the canvas bounds and pass it to the sahder
    glUseProgram(shaderProgram);    // Be sure to call this before doing operations on the shader such as passing projections to it
    float projection[16];
    ortho(canvasBounds.left, canvasBounds.right, canvasBounds.bottom, canvasBounds.top, -1, 1, projection);
    GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
}

void OpenGlRenderer::addToScene(const std::unordered_map<std::string, std::vector<Renderable*>> renderBuckets) {
    for (const auto& [renderTag, renderBucket] : renderBuckets) {
        if(renderBucket.size() > 0) {
            setupGeometry(renderTag, renderBucket);
        }
    }
}

void OpenGlRenderer::renderFrame(const std::unordered_map<std::string, std::pair<Transform, std::vector<Renderable*>>> renderObjects) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderProgram);
    
    // Get uniform location
    GLint offsetLocation = glGetUniformLocation(shaderProgram, "uOffset");
    GLint colorLocation = glGetUniformLocation(shaderProgram, "uColor");
    for(const auto& [renderTag, renderBucket] : renderObjects) {
        // Let's apply the transform and draw
        RenderingContext context = renderingContexts[renderTag];
        glBindVertexArray(context.VAO);
        auto transform = renderBucket.first;
        // The following call can actually happen before the glBindVertexArray, if I understood this correctly.
        // This call merely changes the offset in the shader program itself which is shared between the different
        // shapes. Important is that the correct VAO is bound before we call drawArrays as drawArrays will apply
        // the shared shader (and whateve offset value it currently has) to the vertices of the bound VAO.
        // Making the calls in this order just seems to be kind of a clear and less error prone way of doing this.
        glUniform2f(offsetLocation, transform.x, transform.y);
        
        // Apply the color from the renderable
        auto renderable = renderBucket.second[0];
        glUseProgram(shaderProgram);    // Don't think I need this since I'm already doing it at the top
        glUniform3f(colorLocation, renderable -> color().x, renderable -> color().y, renderable -> color().z);
        
        // FIXME: 6 is hardcoded. 6 refers to the number of vertices to draw a rectangle. This is ugly! This should retrieve the size of the vertex array from the Renderable. Looks like a tomorrow problem to me...
        int numberOfVerticesInRectangle = 6;
        GLsizei nrOfVerticesToDraw = numberOfVerticesInRectangle * (int)renderBucket.second.size();
        glDrawArrays(GL_TRIANGLES, 0, nrOfVerticesToDraw);
    }
    
    glBindVertexArray(0);
}
