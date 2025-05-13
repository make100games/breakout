//
//  GameObject.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "Renderable.hpp"
#include <memory>
#include "Transform.hpp"
#include "Bounds.hpp"
#include "Collider.hpp"
#include "KeyInput.hpp"

// TODO: define a Renderable interface that contains the vertices, color and a renderTag. The renderTag
// will determine what VBO/VAO OpenGL will use to draw it

class GameObject {
public:
    virtual ~GameObject() = default;
    
    /**
    Called once after a game object is initialzed.
     */
    virtual void initialize() = 0;
    
    /**
     Called every frame.
     */
    virtual void update(float deltaTime) = 0;
    
    /**
     Return what should be rendered here. The GameObject must own the Renderable and hold it for the duration of its lifetime.
     */
    virtual const std::unique_ptr<Renderable>& renderable() const = 0;
    
    /**
     The transform of the GameObject.
     */
    virtual Transform transform() = 0;
    
    /**
     Called when the bounds of the canvas have changed. The canvas is the area in the screen in which the entire sceen is drawn.
     */
    virtual void onCanvasBoundsChanged(Bounds bounds) = 0;
    
    /**
     The collider of the GameObject.
     */
    virtual Collider collider() = 0;
    
    /**
     Called when this GameObject's Collider intersects with another GameObject's Collider. The other GameObject's Collider is passed to the method.
     */
    virtual void onCollision(Collider other) = 0;
    
    /**
     Notifies the GameObject of any keyboard inputs.
     */
    virtual void onKeyInput(KeyInput keyInput) = 0;
};

#endif /* GameObject_hpp */
