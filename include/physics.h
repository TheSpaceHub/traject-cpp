#pragma once

#include <cmath>
#include "linalg.h"
#include "rk4.h"
#include "constants.h"
#include "renderer.h"

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

void gravitationalDerivatives(const Matrix<double> &m, Matrix<double> &derivatives)
{
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
    return solver.solve(initialConditions, gravitationalDerivatives, RK4Constants::MAX_STEPS, objectInsideEarth, RK4Constants::SNAPSHOT_FREQUENCY);
}

Matrix<double> simulate(double v, double eastAngle, double groundAngle, const Vector3<double> &initialPos, Vector3<double> &inertialV, RK4Solution &sol, Vector3<double> &finalPos, Matrix<double> &m)
{
    // Given v(km/s), eastAngle (deg * norm), groundAngle (deg) and initialPosition, returns latitude and longitude in a matrix
    // Function needed for energy optimization

    double radLatitude = (Math::pi / 2 - initialPos.phi());
    double radLongitude = initialPos.theta();

    eastAngle *= Math::pi / 180 / Physics::NORM_DEG;
    groundAngle *= Math::pi / 180;

    inertialV =
        localToInertial(radLatitude,
                        radLongitude,
                        v / Physics::NORM_VEL * Vector3<double>(cos(groundAngle) * cos(eastAngle), cos(groundAngle) * sin(eastAngle), sin(groundAngle)));

    sol = getFinalPosition(initialPos, inertialV);

    finalPos = Vector3(sol.solutions(0, 0), sol.solutions(0, 1), sol.solutions(0, 2));

    m(0, 0) = (Math::pi / 2 - finalPos.phi());
    m(1, 0) = (finalPos.theta() - Physics::EARTH_ANGULAR_VELOCITY * sol.steps * RK4Constants::STEP_SIZE);
    return m;
}

RenderObject getVisualTrajectory(RK4Solution &sol, const Vector3<double> &initialPos)
{
    // get the snapshots
    std::vector<Matrix<double>> snapshots = sol.snapshots;

    // create the render objects
    // we will consider orthographic projection
    std::vector<double> x(snapshots.size()), y(snapshots.size()), rndt(snapshots.size()); // data for the multiline
    double xOffset = Graphics::WIDTH / 2;
    double yOffset = Graphics::HEIGHT / 2;

    double midLon = initialPos.theta();
    midLon += Math::pi / 2;

    for (int i = 0; i < snapshots.size(); i++)
    {
        // switch reference frame
        double newX = snapshots[i](0, 0) * cos(midLon) + snapshots[i](0, 1) * sin(midLon);
        double newY = snapshots[i](0, 0) * sin(midLon) - snapshots[i](0, 1) * cos(midLon);

        x[i] = newX / Graphics::metersPerPixel + xOffset;
        y[i] = -snapshots[i](0, 2) / Graphics::metersPerPixel + yOffset;
        rndt[i] = (newY + Graphics::distanceToEarthCenter) / Graphics::metersPerPixel;
    }
    RenderObject earthRender =
        RenderObject::Sphere(Graphics::WIDTH,
                             Graphics::HEIGHT,
                             xOffset,
                             yOffset,
                             Physics::EARTH_RADIUS / Graphics::metersPerPixel,
                             100000 + (Graphics::distanceToEarthCenter + Physics::EARTH_RADIUS) / Graphics::metersPerPixel);
    RenderObject trajectory = RenderObject::Multiline(Graphics::WIDTH, Graphics::HEIGHT, x, y, rndt);
    trajectory.merge(&earthRender);
    return trajectory;
}