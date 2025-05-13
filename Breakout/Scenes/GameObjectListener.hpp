//
//  GameObjectListener.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 11.05.25.
//

#ifndef GameObjectListener_hpp
#define GameObjectListener_hpp

#include <stdio.h>
#include "GameObject.hpp"

class GameObjectListener {
public:
    virtual ~GameObjectListener() = default;
    
    virtual void onRemoveGameObject(GameObject* gameObject) = 0;
};

#endif /* GameObjectListener_hpp */
