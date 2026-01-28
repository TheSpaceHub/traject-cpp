#pragma once

#include <vector>
#include <functional>
#include "linalg.h"

struct RK4Solution
{
    int steps = 0;
    double stepSize;
    double error = 0.0;
    Matrix<double> solutions;

    RK4Solution(int _steps, double _stepSize, double _error, const Matrix<double> &_solutions) : steps(_steps),
                                                                                                 stepSize(_stepSize),
                                                                                                 error(_error),
                                                                                                 solutions(_solutions)
    {
    }
};

class RK4
{
private:
    double h;

public:
    RK4(double stepSize)
    {
        h = stepSize;
    }

    RK4Solution solve(const Matrix<double> &initialConditions, const std::function<void(const Matrix<double> &, Matrix<double> &)> &derivatives, int maxSteps, const std::function<bool(Matrix<double>)> &endCondition)
    {
        // the solver assumes the derivatives do NOT depend on time explicitly
        // input: Matrix (row vector) with initial conditions, Matrix->Matrix (out-parameter), max steps, Matrix->bool to check if we should stop
        Matrix<double> y(initialConditions);
        int n = initialConditions.getCols();
        Matrix<double> k1(1, n), k2(1, n), k3(1, n), k4(1, n);
        int step = 0;
        while (step < maxSteps)
        {
            derivatives(y, k1);
            derivatives(y + h * k1 * 0.5, k2);
            derivatives(y + h * k2 * 0.5, k3);
            derivatives(y + h * k3, k4);

            y = y + h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);

            if (endCondition(y))
                break;
            step++;
        }

        return RK4Solution(step, h, 0.0, y);
    }
};