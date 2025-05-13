//
//  Engine.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include "Scene.hpp"
#include "Renderer.hpp"
#include "Bounds.hpp"
#include "KeyInput.hpp"
#include "SceneListener.hpp"
#include <chrono>

class Engine : public SceneListener {
public:
    Engine();
    ~Engine();
    
    void start();
    
    /**
     Called in each game loop iteration.
     */
    void update();
    
    /**
     The bounds of the canvas on which we can render our content.
     */
    Bounds canvasBounds();
    
    /**
     Notifies the game engine of a keyboard input.
     */
    void onKeyInput(KeyInput input);
    
    // Methods from the SceneListener interface
    /**
     Notifies the GameEngine that the game objects in the scene have changed and should be fetched and added to the rendered scene again. Call this, for instance, after you have dynamically added a new GameObject to the scene or removed a GameObject from the scene. It will ensure that this change makes it into the rendered scene.
     */
    void onGameObjectsInSceneHaveChanged() override;
    
private:
    std::chrono::high_resolution_clock::time_point lastTime;
    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<Renderer> renderer;
    std::vector<std::unique_ptr<Scene>> scenes;
    Bounds myCanvasBounds;
    std::unique_ptr<KeyCode> currentlyPressedKey;    // Not sure if I like this but it makes working with the key input in the GameObjects much more pleasant and it also makes the API behave the way you kind of expect in a GameEngine
    
    void addScenes();
    void checkForCollisions(std::vector<std::unique_ptr<GameObject>>& gameObjects);
    bool isColliding(const Collider& a, const Collider& b);
    void delegateKeyInputToGameObjects(const KeyInput& input);
    float measureTimeSinceLastUpdate();
};


#endif /* Engine_hpp */
