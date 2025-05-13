//
//  Brick.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 10.05.25.
//

#include "Brick.hpp"
#include <string>
#include "Rectangle.hpp"
#include "Vec3.hpp"
#include <stdio.h>
#include <iostream>

static const std::string renderTag = "Brick";

Brick::Brick(std::vector<float> vertices, Collider collider, GameObjectListener* listener) {
    myRenderable = std::make_unique<Rectangle>(vertices, Vec3 { 1.0f, 0.0f, 0.0f }, renderTag);
    myCollider = collider;
    myListener = listener;
}

Brick::~Brick() {
    
}

void Brick::initialize() {
    
}

void Brick::update(float deltaTime) {
    
}

const std::unique_ptr<Renderable>& Brick::renderable() const {
    return myRenderable;
}

Transform Brick::transform() {
    return myTransform;
}

void Brick::onCanvasBoundsChanged(Bounds bounds) {
    
}

Collider Brick::collider() {
    return myCollider;
}

void Brick::onCollision(Collider other) {
    this -> myListener -> onRemoveGameObject(this);
}

void Brick::onKeyInput(KeyInput keyInput) {
    
}
