#pragma once

#include <functional>
#include "rk4.h"
#include "linalg.h"

void testRK4onxist()
{
    std::function<void(Matrix<double>, Matrix<double> &)> der = [](Matrix<double> m, Matrix<double> &retm)
    {
        retm(0, 0) = 1;
    };
    std::function<bool(Matrix<double>)> reachedhalf = [](Matrix<double> m)
    { return m(0, 0) >= 0.5; };

    RK4 solver = RK4(0.001);
    Matrix<double> init(1, 1);
    init(0, 0) = 0.0;
    RK4Solution sol = solver.solve(init, der, 1005, reachedhalf);
    assert(std::abs(0.5 - sol.solutions(0, 0)) < 1e-6);
    std::cout << "  x=t passed" << std::endl;
}

void testCircularMotion()
{
    std::function<void(Matrix<double>, Matrix<double> &)> der = [](Matrix<double> m, Matrix<double> &retm)
    {
        retm(0, 0) = -2 * pi * m(0, 1);
        retm(0, 1) = 2 * pi * m(0, 0);
    };
    std::function<bool(Matrix<double>)> reachedhalf = [](Matrix<double> m)
    { return m(0, 0) <= -(1 - 1e-6); };

    RK4 solver = RK4(0.001);
    Matrix<double> init(1, 2);
    init(0, 0) = 1.0;
    init(0, 1) = 0.0;
    RK4Solution sol = solver.solve(init, der, 1005, reachedhalf);
    assert(abs(-1 - sol.solutions(0, 0)) < 1e-6 and abs(sol.solutions(0, 1)) < 1e-6);
    std::cout << "  x=cos(2pi t), y=sin(2pi t) passed" << std::endl;
}

void runRK4Tests()
{
    testRK4onxist();
    testCircularMotion();
}