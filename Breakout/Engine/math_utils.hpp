//
//  math_utils.hpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#ifndef math_utils_hpp
#define math_utils_hpp

#include <stdio.h>

/**
 Generates an orghographic projection matrix. Didn't feel like including GLM or whatever for now... I know, cost of carry and such. Sue me.
 */
void ortho(float left, float right, float bottom, float top, float near, float far, float* matrix);

#endif /* math_utils_hpp */
