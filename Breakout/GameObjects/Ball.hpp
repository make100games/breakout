//
//  Ball.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 07.05.25.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>
#include "GameObject.hpp"
#include "Renderable.hpp"
#include "Transform.hpp"
#include "Bounds.hpp"
#include <memory>
#include "Collider.hpp"
#include "KeyInput.hpp"

class Ball : public GameObject {
public:
    Ball();
    ~Ball();
    
    void initialize() override;
    
    void update(float deltaTime) override;
    
    const std::unique_ptr<Renderable>& renderable() const override;
    
    Transform transform() override;
    
    void onCanvasBoundsChanged(Bounds bounds) override;
    
    Collider collider() override;
    
    void onCollision(Collider other) override;
    
    void onKeyInput(KeyInput keyInput) override;
    
private:
    std::unique_ptr<Renderable> myRenderable;
    Transform myTransform;
    Bounds myCanvasBounds;
    Collider myCollider;
    float xVelocity = 0.0f;
    float yVelocity = 0.0f;
    float width = 20.0;
    float height = 20.0f;
    
    void performAngleBasedBounce(Collider other);
};

#endif /* Ball_hpp */
