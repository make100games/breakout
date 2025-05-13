//
//  KeyInput.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 08.05.25.
//

#ifndef KeyInput_hpp
#define KeyInput_hpp

#include <stdio.h>

enum class KeyInteraction {
    KeyDown,
    KeyUp
};

enum class KeyCode {
    LeftArrow,
    RightArrow,
    Space
};

struct KeyInput {
    KeyInteraction interaction;
    KeyCode keyCode;
};

#endif /* KeyInput_hpp */
