#include <iostream>
#include <cassert>
#include <vector>
#include "../linalg.h"

#define pi 3.141592653589793238

void testVector3Equality()
{
    Vector3<int> a;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    Vector3<int> b;
    b[0] = 1;
    b[1] = 2;
    b[2] = 3;

    assert(a == b);
    std::cout << "  equality passed" << std::endl;
}

void testVector3FromVector()
{
    Vector3<int> a;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    Vector3<int> b(1, 2, 3);
    assert(a == b);
    std::cout << "  vector3 from vector passed" << std::endl;
}

void testVector3Addition()
{
    Vector3<int> a(1, 1, 1);
    Vector3<int> b(2, 2, 2);

    Vector3<int> c(3, 3, 3);

    assert(a + b == c);
    std::cout << "  addition passed" << std::endl;
}

void testVector3Substraction()
{
    Vector3<int> a(1, 1, 1);
    Vector3<int> b(2, 2, 2);

    Vector3<int> c(3, 3, 3);

    assert(c - b == a);
    std::cout << "  substraction passed" << std::endl;
}

void testVector3Dot()
{
    Vector3<int> a(1, 2, 3);
    Vector3<int> b(0, -1, 3);

    assert(a * b == 7);
    std::cout << "  dot passed" << std::endl;
}

void testVector3Cross()
{
    Vector3<int> i(1, 0, 0);
    Vector3<int> j(0, 1, 0);
    Vector3<int> k(0, 0, 1);

    assert(cross(i, j) == k and cross(j, k) == i and cross(k, i) == j);
    std::cout << "  cross passed" << std::endl;
}

void testVector3Scalar()
{
    Vector3<int> a(1, 2, 3);

    assert(a * 2 == 2 * a and a * 2 == Vector3<int>(2, 4, 6));
    std::cout << "  scalar passed" << std::endl;
}

void testVector3Spherical()
{
    Vector3<double> v = Vector3<double>::fromSpherical(1, pi / 4, pi / 4);
    assert(v.like(Vector3<double>(0.5, 0.5, std::sqrt(0.5))));

    Vector3<double> v1(1, 4, -0.4);
    Vector3<double> v2(-1, 4, -0.4);
    Vector3<double> v3(1, 0, -0.4);

    assert(v1.like(Vector3<double>::fromSpherical(v1.r(), v1.theta(), v1.phi())) and
           v2.like(Vector3<double>::fromSpherical(v2.r(), v2.theta(), v2.phi())) and
           v3.like(Vector3<double>::fromSpherical(v3.r(), v3.theta(), v3.phi())));
    std::cout << "  spherical passed" << std::endl;
}

void testVector3Exceptions()
{
    Vector3<int> a(2, 2, 2);
    Vector3<int> b(3, 3, 3);

    try
    {
        a[3] = 1;
        assert(false);
    }
    catch (const std::out_of_range &e)
    {
    }
    std::cout << "  exceptions passed" << std::endl;
}

void runVector3Tests()
{
    testVector3Equality();
    testVector3FromVector();
    // once we are certain we can create a matrix from a 2d vector we may write matrices using 2d vectors for our tests
    testVector3Addition();
    testVector3Substraction();
    testVector3Dot();
    testVector3Cross();
    testVector3Scalar();
    testVector3Spherical();
    testVector3Exceptions();
}
