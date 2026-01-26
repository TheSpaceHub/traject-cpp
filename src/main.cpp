#include <iostream>
#include "renderer.h"
#include "linalg.h"
#include "rk4.h"
#include "physics.h"
#include "constants.h"
#include <cmath>
#include <windows.h>

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
    // returns the position in inertial frame
    double lat, lon;
    std::cout << "Enter launch coordinates in degrees:" << std::endl;
    std::cout << "Latitude: ";
    ccin(lat);
    std::cout << "Longitude: ";
    ccin(lon);

    lat = Math::pi / 2 - lat * Math::pi / 180;
    lon = lon * Math::pi / 180;
    return Vector3<double>::fromSpherical(Physics::EARTH_RADIUS, lon, lat);
}

Vector3<double> getInitialV(double radLatitude, double radLongitude)
{
    // returns the velocity in inertial frame
    double v, eastAngle, groundAngle;
    std::cout << "Enter speed and angle relative to geographical east (counter-clockwise) and relative to ground (both in degrees):" << std::endl;
    std::cout << "Speed: ";
    ccin(v);
    std::cout << "East angle: ";
    ccin(eastAngle);
    std::cout << "Ground angle: ";
    ccin(groundAngle);

    eastAngle *= Math::pi / 180;
    groundAngle *= Math::pi / 180;

    return localToInertial(radLatitude,
                           radLongitude,
                           v * Vector3<double>(cos(groundAngle) * cos(eastAngle),
                                               cos(groundAngle) * sin(eastAngle),
                                               sin(groundAngle)));
}

int main()
{

    Vector3<double> initialPos = getInitialPos();
    Vector3<double> initialV = getInitialV(Math::pi / 2 - initialPos.phi(), initialPos.theta());

    RK4Solution sol = getFinalPosition(initialPos, initialV);

    std::cout << sol.solutions << std::endl;
    std::cout << sol.steps << std::endl;

    Vector3<double> finalPos(sol.solutions(0, 0), sol.solutions(0, 1), sol.solutions(0, 2));
    std::cout << "final latitude: " << 90 - 180 / Math::pi * (finalPos.phi()) << std::endl;
    std::cout << "final longitude: " << 180 / Math::pi * (finalPos.theta() - Physics::EARTH_ANGULAR_VELOCITY * sol.steps * RK4Constants::STEP_SIZE) << std::endl;

    int keepScreenOpen;
    std::cin >> keepScreenOpen;
    return 0;
}
