#include <iostream>
#include "renderer.h"
#include "linalg.h"

int main()
{
    int WIDTH = 30;
    int HEIGHT = 30;
    Renderer r = Renderer(WIDTH, HEIGHT, 0, 0, 100, 100);
    std::cout << "test";
    r.addObject(3);
    r.render();

    Matrix<int> m(3, 5);
    std::cout << m << std::endl;

    int keepScreenOpen;
    std::cin >> keepScreenOpen;
    return 0;
}
