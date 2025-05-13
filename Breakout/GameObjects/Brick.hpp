//
//  Brick.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 10.05.25.
//

#ifndef Brick_hpp
#define Brick_hpp

#include <stdio.h>
#include "GameObject.hpp"
#include <vector>
#include "GameObjectListener.hpp"

class Brick : public GameObject {
public:
    Brick(std::vector<float> vertices, Collider collider, GameObjectListener* listener);
    ~Brick();
    
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
    GameObjectListener* myListener;
    float width = 20.0;
    float height = 20.0f;
};

#endif /* Brick_hpp */
