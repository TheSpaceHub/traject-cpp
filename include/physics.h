#pragma once

#include <cmath>
#include "linalg.h"
#include "rk4.h"
#include "constants.h"

Vector3<double> spinningv(double lat, double lon)
{
    // returns the velocity vector of the point on Earth in the intertial frame of reference
    return cross(Vector3<double>(0, 0, Physics::EARTH_ANGULAR_VELOCITY),
                 Physics::EARTH_RADIUS * Vector3<double>(cos(lat) * cos(lon),
                                                         cos(lat) * sin(lon),
                                                         sin(lat)));
}

Vector3<double> localToInertial(double lat, double lon, Vector3<double> v)
{
    // latitude and longitue in radians
    Matrix<double> m({{-sin(lon), cos(lon), 0},
                      {-sin(lat) * cos(lon), -sin(lat) * sin(lon), cos(lat)},
                      {cos(lat) * cos(lon), cos(lat) * sin(lon), sin(lat)}});

    return m.transpose() * v + spinningv(lat, lon);
}

Vector3<double> inertialToLocal(double lat, double lon, Vector3<double> v)
{
    // latitude and longitue in radians
    Matrix<double> m({{-sin(lon), cos(lon), 0},
                      {-sin(lat) * cos(lon), -sin(lat) * sin(lon), cos(lat)},
                      {cos(lat) * cos(lon), cos(lat) * sin(lon), sin(lat)}});

    return m * (v - spinningv(lat, lon));
}

Matrix<double> gravitationalDerivatives(const Matrix<double> &m)
{
    Matrix<double> derivatives(1, 6);

    // for x, y and z they are vx, vy and vz
    derivatives(0, 0) = m(0, 3);
    derivatives(0, 1) = m(0, 4);
    derivatives(0, 2) = m(0, 5);

    // for vx, vy and vz they are the usual ones
    const double r = sqrt(m(0, 0) * m(0, 0) + m(0, 1) * m(0, 1) + m(0, 2) * m(0, 2));
    const double factor = -Physics::G * Physics::EARTH_MASS / (r * r * r);
    derivatives(0, 3) = factor * m(0, 0);
    derivatives(0, 4) = factor * m(0, 1);
    derivatives(0, 5) = factor * m(0, 2);

    return derivatives;
}

bool objectInsideEarth(const Matrix<double> &m)
{
    // matrix to be passed is the one used in rk4
    return m(0, 0) * m(0, 0) + m(0, 1) * m(0, 1) + m(0, 2) * m(0, 2) <
           Physics::EARTH_RADIUS * Physics::EARTH_RADIUS;
}

RK4Solution getFinalPosition(Vector3<double> initialPos, Vector3<double> initialV)
{
    RK4 solver(RK4Constants::STEP_SIZE);
    Matrix<double> initialConditions(1, 6);
    // matrix is (x, y, z, vx, vy, vz) (usual trick for odes)
    initialConditions(0, 0) = initialPos[0];
    initialConditions(0, 1) = initialPos[1];
    initialConditions(0, 2) = initialPos[2];
    initialConditions(0, 3) = initialV[0];
    initialConditions(0, 4) = initialV[1];
    initialConditions(0, 5) = initialV[2];
    return solver.solve(initialConditions, gravitationalDerivatives, 100000, objectInsideEarth);
}
