#pragma once

#include "Vector3.h"

/**
 * An accelerometer measures proper acceleration (including gravity) in m/s²,
 * on three axes.
 */

class Accelerometer {
public:
    // Earth's gravity in m/s^2
    static constexpr float STANDARD_GRAVITY = 9.80665F;

    // Get the current acceleration vector, in m/s^2
    virtual Vector3 getAcceleration() = 0;
};
