#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include "linalg.h"
#include "constants.h"
#include "physics.h"
#include "rk4.h"

struct vAngle
{
    double v;
    double eastAngle;

    vAngle(double _v, double _e) : v(_v), eastAngle(_e) {};
};

vAngle getInputs(double groundAngle, const Vector3<double> &initialPos, const Vector3<double> &finalPos)
{

    std::cout << "\n Evaluating for ground angle " << groundAngle << std::endl;
    // given a ground angle and initial and final positions, returns the velocity and east angle needed for a trajectory with those properties

    // define all custom-class variables outside of computationally intensive loops
    Vector3<double> inertialV;
    RK4Solution sol(0, 0, 0, Matrix<double>(1, 1));
    Vector3<double> finalSimulatedPos;
    Matrix<double> m(2, 1);
    Matrix<double> dv(2, 1);
    Matrix<double> deA(2, 1);
    Matrix<double> J(2, 2);

    // initial guess spherical math https://encyclopedai.stavros.io/entries/forward-azimuth/
    double lat1 = Math::pi / 2 - initialPos.phi();
    double lon1 = initialPos.theta();
    double lat2 = Math::pi / 2 - finalPos.phi();
    double lon2 = finalPos.theta();
    double dlon = lon2 - lon1;
    double initialAngle = std::fmod(-180 / Math::pi * atan2(sin(dlon) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon)) + 450, 360);

    vAngle x(Physics::INITIAL_VELOCITY_GUESS * Physics::NORM_VEL, initialAngle * Physics::NORM_DEG);

    Matrix<double> y = simulate(x.v, x.eastAngle, groundAngle, initialPos, inertialV, sol, finalSimulatedPos, m);
    Matrix<double> goal(2, 1);
    goal(0, 0) = (Math::pi / 2 - finalPos.phi());
    goal(1, 0) = finalPos.theta();

    int nonInvertibleJacobianCount = 0;

    while (((y - goal).transpose() * (y - goal))(0, 0) > Physics::LOCATION_TOLERANCE)
    {

        // get new guess by assuming linear function
        dv = simulate(x.v + Math::eps, x.eastAngle, groundAngle, initialPos, inertialV, sol, finalSimulatedPos, m);
        deA = simulate(x.v, x.eastAngle + Math::eps, groundAngle, initialPos, inertialV, sol, finalSimulatedPos, m);

        // simulate (we do this after calculating differentials to have sol in memory to avoid recalculating for visualization)
        y = simulate(x.v, x.eastAngle, groundAngle, initialPos, inertialV, sol, finalSimulatedPos, m);

        // substract y
        dv = dv - y;
        deA = deA - y;
        J(0, 0) = dv(0, 0);
        J(1, 0) = dv(1, 0);
        J(0, 1) = deA(0, 0);
        J(1, 1) = deA(1, 0);

        J = 1 / Math::eps * J;

        if (std::abs(J.det()) < Math::DETERMINANT_ZERO) // TODO: add this to constants
        {
            std::cout << "Warning: optimization process encountered non-invertible Jacobian\n";
            std::cout << "v(norm): " << x.v << std::endl;
            std::cout << "East angle: " << x.eastAngle << std::endl;
            std::cout << "Ground angle: " << groundAngle << std::endl
                      << std::endl;
            // hopefully just a bad initial guess so we modify it
            x.eastAngle = std::fmod(x.eastAngle + 30.0 * Physics::NORM_DEG, 360 * Physics::NORM_DEG);

            nonInvertibleJacobianCount++;
            if (nonInvertibleJacobianCount > 3)
            {
                std::cout << "Non-invertible Jacobian count exceeded.\n";
                break;
            }
            continue;
        }

        // this helps the process take smaller steps whenever the difference is very large
        // the jacobian will vary greatly and the linear approximation will take our object to mars
        double convCoeff = std::max(0.1, exp(-Physics::CONVERGENGE_COEFFICIENT * ((y - goal).transpose() * (y - goal))(0, 0)));

        Matrix<double> delta = J.inverse() * (goal - y);
        x.v += delta(0, 0) * convCoeff;
        x.eastAngle += delta(1, 0) * convCoeff * convCoeff;

        // stores any warning the user needs to know about
        std::string warningString = "";

        // handle orbital speed
        if (x.v > 7909 * Physics::NORM_VEL)
            warningString += "Warning: orbital speeds reached\n";
        // handle negative speed
        if (x.v < 0)
        {
            // trust that our program will not crash
            warningString += "Warning: negative speeds reached\n";
            x.v *= -1;
            x.eastAngle += 180 * Physics::NORM_DEG;
        }
        // handle angle wrapping
        x.eastAngle = std::fmod(x.eastAngle, 360 * Physics::NORM_DEG);

        // draw trajectory and display information
        Renderer renderer(Graphics::WIDTH, Graphics::HEIGHT);
        RenderObject trajectory = getVisualTrajectory(sol, initialPos);
        renderer.addObjectToBuffer(&trajectory);
        renderer.render();
        std::cout << "  Current error: " << std::sqrt(((y - goal).transpose() * (y - goal))(0, 0)) * Physics::EARTH_RADIUS << "m       time: " << sol.steps * RK4Constants::STEP_SIZE << "s" << "       speed: " << x.v / Physics::NORM_VEL << "m/s" << std::endl;
        std::cout << warningString;
    }
    x.v /= Physics::NORM_VEL;
    x.eastAngle /= Physics::NORM_DEG;
    return x;
}

Vector3<double> optimizeTrajectory(Vector3<double> initialPos, Vector3<double> finalPos, double m)
{
    // given positions, returns best velocity

    // slightly cheating by using Vector3 to store non-cartesian coords
    std::vector<double> bestAngles = {}; // ground angles
    std::vector<vAngle> inputs = {};     // will store the velocities and eastAngles
    std::vector<double> energies = {};

    // we make initial guesses
    bestAngles.push_back(35);
    inputs.push_back(getInputs(35, initialPos, finalPos));
    energies.push_back(m * inputs[0].v * inputs[0].v / 2);

    bestAngles.push_back(45);
    inputs.push_back(getInputs(45, initialPos, finalPos));
    energies.push_back(m * inputs[1].v * inputs[1].v / 2);

    bestAngles.push_back(55);
    inputs.push_back(getInputs(55, initialPos, finalPos));
    energies.push_back(m * inputs[2].v * inputs[2].v / 2);

    // we will iterate through the minimums of the parabolas formed by our 3 best guesses until we sort of converge
    int maxIndex = 0, minIndex = 0;
    for (int i = 1; i < 3; i++)
    {
        if (energies[i] < energies[minIndex])
            minIndex = i;
        if (energies[i] > energies[maxIndex])
            maxIndex = i;
    }
    while (energies[maxIndex] - energies[minIndex] > Physics::ENERGY_TOLERANCE)
    {
        // obtain new guess
        // this could be simplified but i do not want to
        const double d01 = (energies[0] - energies[1]) / (bestAngles[0] - bestAngles[1]);
        const double d02 = (energies[0] - energies[2]) / (bestAngles[0] - bestAngles[2]);
        const double a = (d01 - d02) / (bestAngles[1] - bestAngles[2]);
        double newAngle = (-d01 / a + bestAngles[0] + bestAngles[1]) / 2;
        vAngle newvAngle = getInputs(newAngle, initialPos, finalPos);

        // run max to get replaceable angle
        maxIndex = 0;
        for (int i = 1; i < 3; i++)
        {
            if (energies[i] > energies[maxIndex])
                maxIndex = i;
        }
        // replace it
        if (energies[maxIndex] > m * newvAngle.v * newvAngle.v / 2)
        {
            bestAngles[maxIndex] = newAngle;
            inputs[maxIndex] = newvAngle;
            energies[maxIndex] = m * newvAngle.v * newvAngle.v / 2;
        }
        else
        {
            std::cout << "The minimum has been already found, could not optimize until required energy tolerance\n";
            break;
        }

        // run max and min to check for tolerance
        maxIndex = 0, minIndex = 0;
        for (int i = 1; i < 3; i++)
        {
            if (energies[i] < energies[minIndex])
                minIndex = i;
            if (energies[i] > energies[maxIndex])
                maxIndex = i;
        }
    }
    return Vector3<double>(inputs[minIndex].v, inputs[minIndex].eastAngle, bestAngles[minIndex]);
}