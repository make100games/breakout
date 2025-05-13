//
//  Level1.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#ifndef Level1_hpp
#define Level1_hpp

#include <stdio.h>
#include "Scene.hpp"
#include <vector>
#include <memory>
#include "Bounds.hpp"
#include "GameObjectListener.hpp"

class Level1 : public Scene, public GameObjectListener {
public:
    Level1();
    ~Level1();
    
    std::vector<std::unique_ptr<GameObject>>& gameObjects() override;
    
    void onCanvasBoundsChanged(Bounds bounds) override;
    
    void onStart() override;
    
    void update() override;
    
    void addSceneListener(SceneListener* listener) override;
    
    // Methods from GameObjectListener interface
    void onRemoveGameObject(GameObject* gameObject) override;
    
private:
    SceneListener* sceneListener;
    Bounds myCanvasBounds;
    std::vector<std::unique_ptr<GameObject>> myGameObjects;
    std::vector<GameObject*> toBeDeleted;
    
    void addBricks(std::vector<std::unique_ptr<GameObject>>& gameObjects, float boundsWidth, int numberOfColumns, int numberOfRows, float spaceBetweenBricks);
};

#endif /* Level1_hpp */
