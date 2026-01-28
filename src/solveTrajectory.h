#pragma once

#include <iostream>
#include <cmath>
#include "renderer.h"
#include "linalg.h"
#include "rk4.h"
#include "physics.h"
#include "constants.h"
#include "userinput.h"
#include "trajectoryoptimization.h"

void solveTrajectory()
{
    Vector3<double> initialPos = getInitialPos();
    Vector3<double> finalPos = getFinalPos();

    // add mass (just for energy conversions)
    double m;
    std::cout << "Enter mass (kg): ";
    ccinDouble(m);

    Vector3<double> minVelocity = optimizeTrajectory(initialPos, finalPos, m);

    std::cout << "\nMinimum velocity (local coordinates): " << std::endl;
    std::cout << "Speed: " << minVelocity[0] << "m/s" << std::endl;
    std::cout << "Local direction angle: " << minVelocity[1] << (char)248 << " (0" << (char)248 << " facing east, increases counterclockwise)" << std::endl;
    std::cout << "Local elevation angle: " << minVelocity[2] << (char)248 << std::endl;
}
