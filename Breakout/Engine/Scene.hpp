//
//  Scene.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include <vector>
#include "GameObject.hpp"
#include "SceneListener.hpp"

class Scene {
public:
    virtual ~Scene() = default;
    
    /**
     Called when the bounds of the canvas have changed. The canvas is the area in the screen in which the entire sceen is drawn. Initially this is called before any of the other methods in the Scene are called and it can be called any number of times afterwards.
     */
    virtual void onCanvasBoundsChanged(Bounds bounds) = 0;
    
    /**
     Called after the very first time the canvas bounds have changed and before the GameObjects are queried for the first time.
     */
    virtual void onStart() = 0;
    
    /**
     Called each frame. Note: your scene DOES NOT AND SHOULD NOT CALL update() OF ITS GameObjects. This is done by the GameEngine itself. This method merely gives your scene a chance to update anything that might need to update from time to time.
     */
    virtual void update() = 0;
    
    /**
     Called the very first time after onStart() has been called. After that this is called every frame.
     
     A C++ n00b note: Has to be a vector of GameObject pointers. Otherwise you will get the Allocation issue due to object slicing. Holy bananas, Spiderman. Beginner stuff for C++ experts but I'm a n00b here so cut me some slack... Also, needs to be a reference to said vector so that the Scene can continue to own the vector. The reason why the scene should continue to own the vector is because if the scene wants to hold the game objects in a private variable (to avoid recreating the list each time this method is called), this variable would get copied by this method and the copy would be returned. Copying a vector means each entry is copied. unique_ptrs cannot be copied however (as they are, as the name suggests, unique, duh). This attempted copying of a unique_ptr leads to a compiler error (construct_at or something like that).
     */
    virtual std::vector<std::unique_ptr<GameObject>>& gameObjects() = 0;
    
    virtual void addSceneListener(SceneListener* listener) = 0;
};

#endif /* Scene_hpp */
