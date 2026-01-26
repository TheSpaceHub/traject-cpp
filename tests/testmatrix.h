#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include "../linalg.h"

void testMatrixEquality()
{
    Matrix<int> a(2, 2);
    a(0, 0) = 1;
    a(0, 1) = 2;
    a(1, 0) = 3;
    a(1, 1) = 4;
    Matrix<int> b(2, 2);
    b(0, 0) = 1;
    b(0, 1) = 2;
    b(1, 0) = 3;
    b(1, 1) = 4;

    assert(a == b);
    std::cout << "  equality passed" << std::endl;
}

void testMatrixFromVector()
{
    Matrix<int> a({{1, 2},
                   {3, 4}});
    Matrix<int> b(2, 2);
    b(0, 0) = 1;
    b(0, 1) = 2;
    b(1, 0) = 3;
    b(1, 1) = 4;
    assert(a == b);
    std::cout << "  matrix from vector passed" << std::endl;
}

void testMatrixAddition()
{
    Matrix<int> a({{1, 1}});
    Matrix<int> b({{2, 2}});

    Matrix<int> c({{3, 3}});

    assert(a + b == c);
    std::cout << "  addition passed" << std::endl;
}

void testMatrixSubstraction()
{
    Matrix<int> a({{1, 1}});
    Matrix<int> b({{2, 2}});

    Matrix<int> c({{3, 3}});

    assert(c - b == a);
    std::cout << "  substraction passed" << std::endl;
}

void testMatrixMultiplication()
{
    Matrix<int> a({{2, 1},
                   {-1, -3}});
    Matrix<int> b({{0, 4},
                   {2, 7}});

    Matrix<int> c({{2, 15},
                   {-6, -25}});

    assert(a * b == c);

    std::vector<std::vector<int>> v1 = {
        {1, 2, 3}};
    std::vector<std::vector<int>> v2 = {
        {1}, {2}, {3}};

    std::vector<std::vector<int>> v3 = {
        {14}};
    std::vector<std::vector<int>> v4 = {
        {1, 2, 3},
        {2, 4, 6},
        {3, 6, 9}};

    assert(Matrix<int>(v1) * Matrix<int>(v2) == Matrix<int>(v3));
    assert(Matrix<int>(v2) * Matrix<int>(v1) == Matrix<int>(v4));
    std::cout << "  multiplication passed" << std::endl;
}

void testMatrixScalar()
{
    Matrix<int> a({{1, 0},
                   {0, 1}});
    Matrix<int> b({{2, 0},
                   {0, 2}});

    assert(a * 2 == b and 2 * a == b);
    std::cout << "  scalar passed" << std::endl;
}

void testMatrixIdentity()
{
    Matrix<int> a({{1, 0},
                   {0, 1}});
    assert(a == Matrix<int>::id(2));
    std::cout << "  identity passed" << std::endl;
}

void testMatrixTranspose()
{
    Matrix<int> a({{1, 2, 3},
                   {4, 5, 6}});
    Matrix<int> b({{1, 4},
                   {2, 5},
                   {3, 6}});
    assert(a.transpose() == b);
    std::cout << "  transpose passed" << std::endl;
}

void testMatrixDeterminant()
{
    Matrix<int> a({{1, 2},
                   {3, 4}});
    Matrix<int> b({{1, 2},
                   {2, 4}});
    assert(a.det() == -2 and b.det() == 0);
    std::cout << "  determinant passed" << std::endl;
}

void testMatrixInverse()
{
    Matrix<double> a({{1, 2, 3},
                      {3, 4, 5},
                      {8, 10, 0}});
    assert((a * a.inverse()).like(a.inverse() * a) and (a * a.inverse()).like(Matrix<double>::id(3)));
    std::cout << "  inverse passed" << std::endl;
}

void testMatrixExceptions()
{
    Matrix<int> a(2, 2);
    Matrix<int> b(3, 3);

    try
    {
        Matrix<int> c = a * b;
        assert(false);
    }
    catch (const std::invalid_argument &e)
    {
    }
    try
    {
        Matrix<int> c = a + b;
        assert(false);
    }
    catch (const std::invalid_argument &e)
    {
    }
    try
    {
        Matrix<int> c = a - b;
        assert(false);
    }
    catch (const std::invalid_argument &e)
    {
    }
    std::cout << "  exceptions passed" << std::endl;
}

void testMatrixVectorMultiplication()
{
    Matrix<int> m({{2, 0, 0},
                   {0, 3, 0},
                   {0, 0, 1}});
    Vector3<int> v(7, 6, 2);
    assert(m * v == Vector3<int>(14, 18, 2));
    std::cout << "  matrix-vector multiplication passed" << std::endl;
}

void runMatrixTests()
{
    testMatrixEquality();
    testMatrixFromVector();
    // once we are certain we can create a matrix from a 2d vector we may write matrices using 2d vectors for our tests
    testMatrixAddition();
    testMatrixSubstraction();
    testMatrixMultiplication();
    testMatrixScalar();
    testMatrixIdentity();
    testMatrixTranspose();
    testMatrixDeterminant();
    testMatrixInverse();
    testMatrixExceptions();

    std::cout << "Running Matrix and Vector3 combined tests" << std::endl;
    testMatrixVectorMultiplication();
}
