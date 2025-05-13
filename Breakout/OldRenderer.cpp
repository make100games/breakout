//
//  Renderer.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 28.04.25.
//
#define GL_SILENCE_DEPRECATION
#include "OldRenderer.hpp"
#include <OpenGL/gl3.h>
#include <iostream>
#include <chrono>
#include <cmath>

static const char* vertextShaderSource = R"(
#version 330 core

layout (location = 0) in vec2 aPos;
uniform vec2 uOffset;
out vec3 vColor;

void main() {
    gl_Position = vec4(aPos + uOffset, 0.0, 1.0);
    vColor = vec3(uOffset.x + 0.5, 0.0, 0.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core

in vec3 vColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

double getTimeSeconds() {
    using namespace std::chrono;
    static auto start = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    duration<double> elapsed = now - start;
    return elapsed.count();
}

OldRenderer::OldRenderer()
    : VAO1(0), VBO1(0), VAO2(0), VBO2(0), shaderProgram(0){
    
}

OldRenderer::~OldRenderer() {
    if (shaderProgram) glDeleteProgram(shaderProgram);
    if (VBO1) glDeleteBuffers(1, &VBO1);
    if (VAO1) glDeleteVertexArrays(1, &VAO1);
    if (VBO2) glDeleteBuffers(1, &VBO2);
    if (VAO2) glDeleteVertexArrays(1, &VAO2);
}

void OldRenderer::initialize() {
    setupShader();
    setupGeometry();
    glClearColor(0.2f, 0.4f, 0.6f, 1.0f); // Background color
}

void OldRenderer::setupShader() {
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

void OldRenderer::setupGeometry() {
    // Rectangle made of 2 triangles
    float vertices[] = {
        // Triangle 1
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        // Triangle 2
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
    // Smaller square
    float rect2Vertices[] = {
        -0.1f, -0.1f,
         0.1f, -0.1f,
         0.1f,  0.1f,
        -0.1f, -0.1f,
         0.1f,  0.1f,
        -0.1f,  0.1f
    };
    
    // TODO: We should probably keep a list of VAOs and VBOs instead of duplicating all of this error prone setup
    // here... OR at the very least create a function that does this...
    // Generate and bind VBOs and VAOs and upload the vertices to the GPU and such
    // *********** VBO1 and VAO1 ***********
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    
    // This call here actually copies the vertices declared above to the GPUs memory. This makes use of the
    // currently bound VBO and VAO hence you don't need to specify them explicitly here. Yay for OpenGL being
    // a state machine. Not...
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Define how the shader should read the vertex data. This, again, considers the currently bound VAO and VBO
    glVertexAttribPointer(
        0,                  // Attribute location in shader (layout(location = 0))
        2,                  // Number of components (e.g. vec2 → 2)
        GL_FLOAT,           // Type
        GL_FALSE,           // Normalize?
        2 * sizeof(float),  // Stride (distance between vertices)
        (void*)0            // Offset to first component
    );
    glEnableVertexAttribArray(0);   // Atribute 0 = 'position'
    
    // Reset things to ensure a clean state. OpenGL is a state machine. Make
    // sure we reset the state so future operations don't start with wrong
    // assumptions
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // *********** VBO1 and VAO1 ***********
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect2Vertices), rect2Vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(
        0,                  // Attribute location in shader (layout(location = 0))
        2,                  // Number of components (e.g. vec2 → 2)
        GL_FLOAT,           // Type
        GL_FALSE,           // Normalize?
        2 * sizeof(float),  // Stride (distance between vertices)
        (void*)0            // Offset to first component
    );
    
    glEnableVertexAttribArray(0);   // Atribute 0 = 'position'
    
    // Reset things to ensure a clean state. OpenGL is a state machine. Make
    // sure we reset the state so future operations don't start with wrong
    // assumptions
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OldRenderer::renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    double myTime = getTimeSeconds();
    float offset = sin(myTime);
    
    glUseProgram(shaderProgram);
    
    // Get uniform location
    GLint offsetLocation = glGetUniformLocation(shaderProgram, "uOffset");
    
    // Let's apply the transform and draw
    glBindVertexArray(VAO1);
    // The following call can actually happen before the glBindVertexArray, if I understood this correctly.
    // This call merely changes the offset in the shader program itself which is shared between the different
    // shapes. Important is that the correct VAO is bound before we call drawArrays as drawArrays will apply
    // the shared shader (and whateve offset value it currently has) to the vertices of the bound VAO.
    // Making the calls in this order just seems to be kind of a clear and less error prone way of doing this.
    glUniform2f(offsetLocation, offset, 0.0); // Move first shape on x axis
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(VAO2);
    glUniform2f(offsetLocation, 0.0, offset);  // move second shape on y axis
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}
