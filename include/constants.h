#pragma once

namespace RK4Constants
{
    const double STEP_SIZE = 1e-3;
    const int MAX_STEPS = 1e8;
    const int SNAPSHOT_FREQUENCY = 1e5; // inverse of frequency but yeah
}

namespace Math
{
    const double pi = 3.141592653589793238;
    const double eps = 1e-2;
    const double DETERMINANT_ZERO = 1e-12;

    constexpr bool SAFE_MATRICES = false;
}

namespace Physics
{

    // normalization constants required to keep computer happy
    // consider them "units"
    const double NORM_VEL = 1e-3;
    const double NORM_DEG = 1;

    const double CONVERGENGE_COEFFICIENT = 1;

    const double EARTH_MASS = 5.972e24;
    const double EARTH_RADIUS = 6371000;
    const double EARTH_ANGULAR_VELOCITY = 7.29e-5;
    const double G = 6.674e-11;
    const double ENERGY_TOLERANCE = 0.1;
    const double LOCATION_TOLERANCE = 1e-12;    // in squared degrees
    const double INITIAL_VELOCITY_GUESS = 7000; // m/s
}

namespace Graphics
{
    const int WIDTH = 100;
    const int HEIGHT = 60;
    const double distanceToEarthCenter = 6371000 * 3; // in meters; 3 * r
    const double metersPerPixel = 6371000 / 15;       // diameter of 30 pixels
}