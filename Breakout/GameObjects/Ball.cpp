//
//  Ball.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 07.05.25.
//

#include "Ball.hpp"
#include <string>
#include "Rectangle.hpp"
#include <vector>
#include <iostream>

static const std::string tag = "Ball";
static const float startXVeloc = 200.0f;
static const float startYVeloc = 500.0f;

Ball::Ball() : myTransform(Transform { 0.0f, 500.0f, tag }) {
    // Note: Place top left vertex at 0,0. Otherwise you bake in an offset. If you want to position an object, do so in the initialize() method by changing its transform
    float top = 0.0f;
    float left = 0.0f;
    std::vector<float> vertices = {
        //Tri 1
        top, left,
        top + height, left,
        top + height, left + width,
        //Tri 2
        top, left,
        top, left + width,
        top + height, left + width
    };
    Vec3 color = Vec3 { 1.0f, 1.0f, 1.0f };
    myRenderable = std::make_unique<Rectangle>(vertices, color, tag);
}

Ball::~Ball() {
    
}

void Ball::initialize() {
    myCollider = Collider { myTransform.x, myTransform.y, width, height };
}

void Ball::update(float deltaTime) {
    myTransform = myTransform.copyWithX(myTransform.x + (xVelocity * deltaTime)).copyWithY(myTransform.y + (yVelocity * deltaTime));
    if(myTransform.x + width > myCanvasBounds.right) {
        xVelocity *= -1;
        myTransform.x = myCanvasBounds.right - width;
    }
    if(myTransform.x < myCanvasBounds.left) {
        xVelocity *= -1;
        myTransform.x = myCanvasBounds.left;
    }
    if(myTransform.y + height > myCanvasBounds.bottom) {
        yVelocity *= -1;
        myTransform.y = myCanvasBounds.bottom - height;
    }
    if(myTransform.y < myCanvasBounds.top) {
        yVelocity *= -1;
        myTransform.y = myCanvasBounds.top;
    }
    myCollider = myCollider.copyWithX(myTransform.x).copyWithY(myTransform.y);
}

const std::unique_ptr<Renderable>& Ball::renderable() const {
    return myRenderable;
}

Transform Ball::transform() {
    return myTransform;
}

void Ball::onCanvasBoundsChanged(Bounds bounds) {
    myCanvasBounds = bounds;
}

Collider Ball::collider() {
    return myCollider;
}

void Ball::onCollision(Collider other) {
    // Do a simple overlap resolution to see whether the ball hit the sides of an object or the top/bottom of it. If it hit the sides, invert the x velocity. If it hit the top/bottom, invert the y velocity. Courtesy of Chat GPT :)
    
    Collider thisCollider = myCollider; // Get the ball's current collider bounds

    // Compute the centers
    float thisCenterX = thisCollider.x + thisCollider.width / 2.0f;
    float thisCenterY = thisCollider.y + thisCollider.height / 2.0f;
    float otherCenterX = other.x + other.width / 2.0f;
    float otherCenterY = other.y + other.height / 2.0f;

    // Compute the current overlap (assuming axis-aligned rectangles)
    float deltaX = thisCenterX - otherCenterX;
    float deltaY = thisCenterY - otherCenterY;

    float combinedHalfWidths = (thisCollider.width + other.width) / 2.0f;
    float combinedHalfHeights = (thisCollider.height + other.height) / 2.0f;

    float overlapX = combinedHalfWidths - std::abs(deltaX);
    float overlapY = combinedHalfHeights - std::abs(deltaY);

    // Determine direction of collision
    if (overlapX < overlapY) {
        // Horizontal collision → invert X velocity
        xVelocity *= -1;
        if(deltaX > 0) {
            // Collided with right side
            // Position ball at the edge of collision to avoid spurious double collisions? Not sure if this will do anything but it's an effort to fix some bugs where the ball sometimes just plows through multiple rows of bricks
            myTransform.x = other.x + other.width;
        } else if(deltaX < 0) {
            myTransform.x = other.x;
        }
    } else {
        if (other.tag == "Paddle") {
            performAngleBasedBounce(other);
        } else {
            // Vertical collision → invert Y velocity
            yVelocity *= -1;
            if (deltaY > 0) {
                // Collided with bottom edge
                myTransform.y = other.y + other.height;
            } else if(deltaY < 0) {
                // Colided with top edge
                myTransform.y = other.y;
            }
        }
    }
}

void Ball::performAngleBasedBounce(Collider other) {
    // The following is also courtesy of Chat GPT. Gettin' a little too comfortable having it do all of the algorithmic heavy lifting for me. But I do understand what is happening here :)
    // This should only be called if we collide with the paddle so we can assume that the other collider is the collider of the paddle
    float ballCenterX = myCollider.x + (myCollider.width / 2);
    float paddleCenterX = other.x + (other.width / 2);
    float paddleHalfWidth = other.width / 2.0f;

    // Calculate the hit position relative to paddle center
    float distanceFromCenter = ballCenterX - paddleCenterX;

    // Normalize to [-1.0, 1.0] range
    float normalized = distanceFromCenter / paddleHalfWidth;
    normalized = std::clamp(normalized, -1.0f, 1.0f); // safety check

    // Add x velocity based on hit location
    float maxBounceAngle = 75.0f * (M_PI / 180.0f); // in radians
    float speed = std::sqrt(xVelocity * xVelocity + yVelocity * yVelocity);
    xVelocity = speed * std::sin(normalized * maxBounceAngle);
    yVelocity = -speed * std::cos(normalized * maxBounceAngle);
}

void Ball::onKeyInput(KeyInput keyInput) {
    // Ball starts moving when player presses space bar
    if (keyInput.interaction == KeyInteraction::KeyDown &&
        keyInput.keyCode == KeyCode::Space) {
        xVelocity = startXVeloc;
        yVelocity = startYVeloc;
    }
}
