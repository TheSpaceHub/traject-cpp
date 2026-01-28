#pragma once

#include <iostream>
#include "linalg.h"
#include "physics.h"

inline void ccinDouble(double &d, int retriesLeft = 3)
{
    // ccinDouble retries the cin if the user makes a mistake
    try
    {
        std::cin >> d;
    }
    catch (...)
    {
        std::cout << "Check the input and try again.\n";
        ccinDouble(d, retriesLeft - 1);
    }
}

inline Vector3<double> getInitialPos()
{
    // returns the position in inertial frame
    double lat, lon;
    std::cout << "Enter launch coordinates in degrees:" << std::endl;
    std::cout << "Latitude: ";
    ccinDouble(lat);
    std::cout << "Longitude: ";
    ccinDouble(lon);

    lat = Math::pi / 2 - lat * Math::pi / 180;
    lon = lon * Math::pi / 180;
    return Vector3<double>::fromSpherical(Physics::EARTH_RADIUS, lon, lat);
}

inline Vector3<double> getFinalPos()
{
    // returns the position in inertial frame
    double lat, lon;
    std::cout << "Enter target coordinates in degrees:" << std::endl;
    std::cout << "Latitude: ";
    ccinDouble(lat);
    std::cout << "Longitude: ";
    ccinDouble(lon);

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
    ccinDouble(v);
    std::cout << "East angle: ";
    ccinDouble(eastAngle);
    std::cout << "Ground angle: ";
    ccinDouble(groundAngle);

    eastAngle *= Math::pi / 180;
    groundAngle *= Math::pi / 180;

    return localToInertial(radLatitude,
                           radLongitude,
                           v * Vector3<double>(cos(groundAngle) * cos(eastAngle),
                                               cos(groundAngle) * sin(eastAngle),
                                               sin(groundAngle)));
}