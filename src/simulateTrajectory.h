#pragma once

#include <iostream>
#include "renderer.h"
#include "linalg.h"
#include "rk4.h"
#include "physics.h"
#include "constants.h"
#include "userinput.h"
#include <cmath>

void simulateTrajectory()
{

    Vector3<double> initialPos = getInitialPos();
    Vector3<double> initialV = getInitialV(Math::pi / 2 - initialPos.phi(), initialPos.theta());

    RK4Solution sol = getFinalPosition(initialPos, initialV);

    Vector3<double> finalPos(sol.solutions(0, 0), sol.solutions(0, 1), sol.solutions(0, 2));
    std::cout << std::endl;
    std::cout << "Final latitude: " << 90 - 180 / Math::pi * (finalPos.phi()) << (char)248 << std::endl;
    std::cout << "Final longitude: " << 180 / Math::pi * (finalPos.theta() - Physics::EARTH_ANGULAR_VELOCITY * sol.steps * RK4Constants::STEP_SIZE) << (char)248 << std::endl;
}
