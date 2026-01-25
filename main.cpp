#include <iostream>
#include "renderer.h"
#include "linalg.h"
#include <cmath>
#include <windows.h>

#define pi 3.141592653589793238

int main()
{
    RenderObject *o = new RenderObject(RenderObject::Circle(30, 30, 10, 0, 7, 1));
    Renderer renderer(30, 30);
    for (int k = 0; k < 30; k++)
    {
        *o = RenderObject::Circle(30, 30, 10, k, 7, 1);
        renderer.addObjectToBuffer(o);
        renderer.render();
        Sleep(50);
    }
    delete o;

    

    int keepScreenOpen;
    std::cin >> keepScreenOpen;
    return 0;
}
