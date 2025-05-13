//
//  Engine.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#include "Engine.hpp"
#include <memory>
#include "Level1.hpp"
#include "OpenGlRenderer.hpp"
#include <iostream>
#include <utility>
#include <unordered_map>

Engine::Engine() : myCanvasBounds(Bounds { 0, 1000, 0, 700 }) {
    lastTime = std::chrono::high_resolution_clock::now();
    renderer = std::make_unique<OpenGlRenderer>();
    scenes = std::vector<std::unique_ptr<Scene>>();
}

Engine::~Engine() {
    scenes.clear();
}

void Engine::start() {
    renderer->initialize(myCanvasBounds);
    addScenes();
    
    if(!scenes.empty()) {
        currentScene = std::move(scenes[0]);
        std::cout << "Current scene obtained\n";
    } else {
        std::cout << "No scenes available\n";
    }
    if(currentScene) {
        currentScene -> addSceneListener(this);
        currentScene -> onCanvasBoundsChanged(myCanvasBounds);
        currentScene -> onStart();
        std::unordered_map<std::string, std::vector<Renderable*>> renderBuckets;
        auto& gameObjects = currentScene -> gameObjects();
        for (auto& go : gameObjects) {
            go -> onCanvasBoundsChanged(myCanvasBounds);
            auto& renderable = go -> renderable();
            auto renderables = renderBuckets[renderable -> renderTag()];
            renderables.push_back(renderable.get());
            renderBuckets[renderable -> renderTag()] = renderables;
            go -> initialize();
            std::cout << "GameObject initialized\n";
        }
        renderer -> addToScene(renderBuckets);
        std::cout << "Current scene loaded\n";
    } else {
        std::cout << "No scene available!\n";
    }
}

void Engine::update() {
    float deltaTime = measureTimeSinceLastUpdate();
    
    if(currentScene) {
        currentScene -> update();
        auto& gameObjects = currentScene -> gameObjects();
        std::unordered_map<std::string, std::pair<Transform, std::vector<Renderable*>>> renderObjects;
        for (auto& go : gameObjects) {
            go -> update(deltaTime);
            auto transform = go -> transform();
            auto& renderable = go -> renderable();
            auto& renderObject = renderObjects[renderable -> renderTag()];
            renderObject.first = transform;
            renderObject.second.push_back(renderable.get());
            renderObjects[renderable -> renderTag()] = renderObject;
        }
        // There is probably a much more efficient way to do all this but I wanted to make sure that I had as few draw calls as possible in the renderer and to do that I batched Renderables that share the same tag into one draw call. Bridging this with the GameObject API (and avoiding having to expose too many of the implementation details of the Renderer to the GameObjects) I have to bring all this together here.
        renderer -> renderFrame(renderObjects);
        checkForCollisions(gameObjects);
    }
}

float Engine::measureTimeSinceLastUpdate() {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = now - lastTime;
    lastTime = now;
    return duration.count();
}

// FIXME: Optimize this to use broad phase and narrow phase collision detection. Right now we just have a nasty O(n squared) nested for-loop situation here
void Engine::checkForCollisions(std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    for (auto& go : gameObjects) {
        bool alreadyCollided = false;
        for (auto& otherGo : gameObjects) {
            // We don't want to check collision with ourselves
            if (go != otherGo) {
                bool collisionOccurred = isColliding(go -> collider(), otherGo -> collider());
                if(collisionOccurred && !alreadyCollided) {
                    go -> onCollision(otherGo -> collider());
                    alreadyCollided = true;
                }
            }
        }
    }
}

// Simple AABB (Axis-Algined Bounding Box) collision detection
bool Engine::isColliding(const Collider& a, const Collider& b) {
    return (
        a.x < b.x + b.width &&
        a.x + a.width > b.x &&
        a.y < b.y + b.height &&
        a.y + a.height > b.y
    );
}

Bounds Engine::canvasBounds() {
    return myCanvasBounds;
}

void Engine::addScenes() {
    scenes.push_back(std::make_unique<Level1>());
}

/**
 We want to make sure that key-up events are only delegated to the game objects if the key that popped back up is also the key that is currently pressed down. Otherwise we just ignore it. This leads to much more intuitive key input handling for games.
 */
void Engine::onKeyInput(KeyInput input) {
    if(input.interaction == KeyInteraction::KeyUp && currentlyPressedKey && *currentlyPressedKey == input.keyCode) {
        delegateKeyInputToGameObjects(input);
        currentlyPressedKey = nullptr;
    } else if(input.interaction == KeyInteraction::KeyDown) {
        delegateKeyInputToGameObjects(input);
        currentlyPressedKey = std::make_unique<KeyCode>(input.keyCode);
    }
}

void Engine::delegateKeyInputToGameObjects(const KeyInput& input) {
    if(currentScene) {
        auto& gameObjects = currentScene -> gameObjects();
        for(auto& go : gameObjects) {
            go -> onKeyInput(input);
        }
    }
}

void Engine::onGameObjectsInSceneHaveChanged() {
    // Here we basically fetch the GameObjects again (since they probably changed, otherwise why would the caller have called this method), and add them anew to the Renderer. This replaces what's already in the Renderer's scene
    // Note: This is a bit of copy/pasta from a snippet above. I could probably find a way to share this somehow but my hope is this won't change frequently. Famous last words, I know...
    std::unordered_map<std::string, std::vector<Renderable*>> renderBuckets;
    auto& gameObjects = currentScene -> gameObjects();
    for (auto& go : gameObjects) {
        auto& renderable = go -> renderable();
        auto renderables = renderBuckets[renderable -> renderTag()];
        renderables.push_back(renderable.get());
        renderBuckets[renderable -> renderTag()] = renderables;
    }
    renderer -> addToScene(renderBuckets);
}
