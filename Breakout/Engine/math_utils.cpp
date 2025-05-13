//
//  math_utils.cpp
//  Breakout
//
//  Created by Chris Braunschweiler on 02.05.25.
//

#include "math_utils.hpp"

// TODO: write unit test to test this function thoroughly
/**
 Calculates values of an orthographic projection matrix and populates the passed matrix with those values. The resulting matrix is
 then usable for passing to, e.g. OpenGL as a projection.
 */
void ortho(float left, float right, float bottom, float top, float near, float far, float* matrix) {
    matrix[0]  = 2.0f / (right - left);
    matrix[1]  = 0.0f;
    matrix[2]  = 0.0f;
    matrix[3]  = 0.0f;

    matrix[4]  = 0.0f;
    matrix[5]  = 2.0f / (top - bottom);
    matrix[6]  = 0.0f;
    matrix[7]  = 0.0f;

    matrix[8]  = 0.0f;
    matrix[9]  = 0.0f;
    matrix[10] = -2.0f / (far - near);
    matrix[11] = 0.0f;

    matrix[12] = -(right + left) / (right - left);
    matrix[13] = -(top + bottom) / (top - bottom);
    matrix[14] = -(far + near) / (far - near);
    matrix[15] = 1.0f;
}
