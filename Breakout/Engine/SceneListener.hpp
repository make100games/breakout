//
//  SceneListener.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 11.05.25.
//

#ifndef SceneListener_hpp
#define SceneListener_hpp

#include <stdio.h>

class SceneListener {
public:
    virtual ~SceneListener() = default;
    
    virtual void onGameObjectsInSceneHaveChanged() = 0;
};

#endif /* SceneListener_hpp */
