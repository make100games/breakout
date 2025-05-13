//
//  Level1.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#include "Level1.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include "Brick.hpp"
#include "Collider.hpp"

Level1::Level1() {
    
}

Level1::~Level1() {
    //Clear game objects, I guess
}

void Level1::onCanvasBoundsChanged(Bounds bounds) {
    myCanvasBounds = bounds;
}

void Level1::onStart() {
    myGameObjects.push_back(std::make_unique<Paddle>());
    myGameObjects.push_back(std::make_unique<Ball>());
    
    // Add brick wall
    float spaceBetweenBricks = 5.0f;
    float boundsWidth = myCanvasBounds.right - myCanvasBounds.left;
    addBricks(myGameObjects, boundsWidth, 20, 5, spaceBetweenBricks);
}

void Level1::update() {
    // Delete any GameObjects that are marked for deletion
    // Note: Would be kind of nice to bake this into the engine somehow since this is probably a pretty common use case. But that would probably require an abstract Scene base class and not just an interface (fully abstract, with only virtual methods) and then we get into more hairy inheritance stuff. I don't want to go into the territory of having too much inheritance (other than interface implementation). But in this case, it may be warranted.
    bool gameObjectsWereDeleted = false;
    auto& gos = gameObjects();
    for(auto goToBeDeleted : toBeDeleted) {
        auto it = std::find_if(gos.begin(), gos.end(), [goToBeDeleted](const std::unique_ptr<GameObject>& ptr) {
            return ptr.get() == goToBeDeleted;
        });
        
        if (it != gos.end()) {
            gos.erase(it);
            gameObjectsWereDeleted = true;
        }
    }
    toBeDeleted.clear();
    if(gameObjectsWereDeleted) {
        this -> sceneListener -> onGameObjectsInSceneHaveChanged();
    }
}

// Courtesy of ChatGPT with some adjustments. Cuz ain't nobody got time to implement something like this. No but seriously, I wanted to focus on the C++ and OpenGL-specific stuff here and did not want to invest too much time in code that I could write, more or less, verbatim in other languages.
void Level1::addBricks(std::vector<std::unique_ptr<GameObject>>& gameObjects, float boundsWidth, int numberOfColumns, int numberOfRows, float spaceBetweenBricks) {
    std::vector<std::unique_ptr<Brick>> bricks;

    if (numberOfColumns <= 0 || numberOfRows <= 0) return;

    // Calculate total spacing and brick size
    float totalSpacingX = spaceBetweenBricks * (numberOfColumns + 1);
    float totalBrickWidth = boundsWidth - totalSpacingX;
    float brickWidth = totalBrickWidth / numberOfColumns;
    float brickHeight = brickWidth * 0.5f; // You can parameterize this if needed

    // Start at top-left corner
    float startX = spaceBetweenBricks;
    float startY = spaceBetweenBricks;

    for (int row = 0; row < numberOfRows; ++row) {
        for (int col = 0; col < numberOfColumns; ++col) {
            float x = startX + col * (brickWidth + spaceBetweenBricks);
            float y = startY + row * (brickHeight + spaceBetweenBricks);

            float x0 = x;
            float y0 = y;
            float x1 = x + brickWidth;
            float y1 = y + brickHeight;

            // Two triangles per brick
            std::vector<float> vertices = {
                x0, y0,  // top-left
                x1, y0,  // top-right
                x1, y1,  // bottom-right

                x0, y0,  // top-left
                x1, y1,  // bottom-right
                x0, y1   // bottom-left
            };
            Collider collider = { x0, y0, brickWidth, brickHeight };
            gameObjects.push_back(std::make_unique<Brick>(vertices, collider, this));
        }
    }
}


std::vector<std::unique_ptr<GameObject>>& Level1::gameObjects() {
    return myGameObjects;
}

void Level1::addSceneListener(SceneListener* listener) {
    this -> sceneListener = listener;
}

void Level1::onRemoveGameObject(GameObject *gameObject) {
    toBeDeleted.push_back(gameObject);
}
