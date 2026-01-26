#include <iostream>
#include "renderer.h"
#include "linalg.h"
#include "rk4.h"
#include "physics.h"
#include <cmath>
#include <windows.h>

#define pi 3.141592653589793238
#define STEP_SIZE 1e-3

void ccin(double &d, int retriesLeft = 3)
{
    // ccin retries the cin if the user makes a mistake
    try
    {
        std::cin >> d;
    }
    catch (...)
    {
        std::cout << "Check the input and try again.\n";
        ccin(d, retriesLeft - 1);
    }
}

Vector3<double> getInitialPos()
{
    double EARTH_RADIUS = 6371000;

    double lat, lon;
    std::cout << "Enter launch coordinates in degrees:" << std::endl;
    std::cout << "Latitude: ";
    ccin(lat);
    std::cout << "Longitude: ";
    ccin(lon);

    lat = pi / 2 - lat * pi / 180;
    lon = lon * pi / 180;
    return Vector3<double>::fromSpherical(EARTH_RADIUS, lon, lat);
}

Vector3<double> getInitialLocalV()
{
    double v, eastAngle, groundAngle;
    std::cout << "Enter speed and angle relative to geographical east (counter-clockwise) and relative to ground (both in degrees):" << std::endl;
    std::cout << "Speed: ";
    ccin(v);
    std::cout << "East angle: ";
    ccin(eastAngle);
    std::cout << "Ground angle: ";
    ccin(groundAngle);

    //return Vector3<double>::fromSpherical(v * sin(), lon, lat);
}

RK4Solution getFinalPosition(Vector3<double> initialPos, Vector3<double> initalLocalV)
{
    RK4 solver(STEP_SIZE);
    Matrix<double> initialConditions(1, 6);
    //return solver.solve();
}

int main()
{

    Vector3<double> initialPos = getInitialPos();
    Vector3<double> initialLocalV = getInitialLocalV();

    RK4 solver(1e-3);

    int keepScreenOpen;
    std::cin >> keepScreenOpen;
    return 0;
}
