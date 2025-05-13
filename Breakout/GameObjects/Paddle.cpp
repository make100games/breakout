//
//  Paddle.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#include "Paddle.hpp"
#include "Rectangle.hpp"

static const std::string tag = "Paddle";
static const float velocity = 500.0f;

Paddle::Paddle():myTransform(Transform { 0.0f, 0.0f, tag }) {
    // Note: Place top left vertex at 0,0. Otherwise you bake in an offset. If you want to position an object, do so in the initialize() method by changing its transform
    float left = 0.0f;
    float top = 0.0f;
    std::vector<float> vertices = {
        // Triangle 1
        left, top,
        left, top + height,
        left + width, top + height,
        // Triangle 2
        left, top,
        left + width, top,
        left + width, top + height
    };
    Vec3 color = Vec3 { 1.0f, 1.0f, 0.0f };
    myRenderable = std::make_unique<Rectangle>(vertices, color, tag);
}

Paddle::~Paddle() {
    
}

void Paddle::initialize() {
    myTransform = myTransform.copyWithY(myCanvasBounds.bottom - height - 50);
    myCollider = Collider { myTransform.x, myTransform.y, width, height, tag };
}

void Paddle::update(float deltaTime) {
    myTransform = myTransform.copyWithX(myTransform.x + (xVelocity * deltaTime));
    if(myTransform.x + width > myCanvasBounds.right) {
        xVelocity = 0.0f;
        myTransform.x = myCanvasBounds.right - width;
    }
    if(myTransform.x < myCanvasBounds.left) {
        xVelocity = 0.0f;
        myTransform.x = myCanvasBounds.left;
    }
    myCollider = myCollider.copyWithX(myTransform.x).copyWithY(myTransform.y);
}

const std::unique_ptr<Renderable>& Paddle::renderable() const {
    return myRenderable;
}

Transform Paddle::transform() {
    return myTransform;
}

void Paddle::onCanvasBoundsChanged(Bounds bounds) {
    myCanvasBounds = bounds;
}

Collider Paddle::collider() {
    return myCollider;
}

void Paddle::onCollision(Collider other) {
    // No-op. Paddle does not need to react to collision with anything
}

void Paddle::onKeyInput(KeyInput keyInput) {
    if(keyInput.interaction == KeyInteraction::KeyDown) {
        switch(keyInput.keyCode) {
            case KeyCode::LeftArrow:
                if(myTransform.x > myCanvasBounds.left) {
                    xVelocity = -velocity;
                }
                break;
            case KeyCode::RightArrow:
                if((myTransform.x + width) < myCanvasBounds.right) {
                    xVelocity = velocity;
                }
                break;
            default:
                break;
        }
    } else if(keyInput.interaction == KeyInteraction::KeyUp) {
        xVelocity = 0.0f;
    }
}
