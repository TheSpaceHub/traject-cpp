#include <iostream>
#include "renderer.h"
#include "linalg.h"
#include <cmath>

#define pi 3.141592653589793238

int main()
{
    int WIDTH = 30;
    int HEIGHT = 30;
    Renderer r = Renderer(WIDTH, HEIGHT, 0, 0, 100, 100);
    std::cout << "test";
    r.addObject(3);
    r.render();

    Vector3<double> v = Vector3<double>::fromSpherical(1, pi / 4, pi / 4);
    std::cout << v << std::endl;

    int keepScreenOpen;
    std::cin >> keepScreenOpen;
    return 0;
}
