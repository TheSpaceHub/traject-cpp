#include <iostream>
#include <cassert>
#include <vector>
#include <windows.h>
#include "../renderer.h"

void displayFadingWhite()
{
    RenderObject *o = new RenderObject(10, 10);
    Renderer renderer(10, 10);

    for (int k = 10; k >= 0; k--)
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                o->set(i, j, 1, (i < k) ? 1 : -1);
            }
        }
        renderer.addObjectToBuffer(o);
        renderer.render();
        Sleep(100);
    }

    delete o;
}
void displayMovingCircle()
{
    for (int k = 0; k < 30; k++)
    {
        RenderObject *o = new RenderObject(RenderObject::Circle(30, 30, 10, k, 7, 1));
        Renderer renderer(30, 30);
        renderer.addObjectToBuffer(o);
        renderer.render();
        delete o;
        Sleep(50);
    }
}

void displayLine()
{
    RenderObject *v = new RenderObject(RenderObject::Circle(30, 30, 10, 5, 7, 5));
    RenderObject *o = new RenderObject(RenderObject::Line(30, 30, 3, 4, 20, 10, 0, 10));
    Renderer renderer(30, 30);
    renderer.addObjectToBuffer(o);
    renderer.addObjectToBuffer(v);
    renderer.render();
    delete o;
}

void testRendererCompatibility()
{
    RenderObject *o = new RenderObject(30, 30);
    RenderObject *v = new RenderObject(30, 20);
    Renderer r(30, 30);
    r.addObjectToBuffer(o);
    try
    {
        r.addObjectToBuffer(v);
        assert(false);
    }
    catch (const std::exception &e)
    {
    }
    std::cout << "  renderer compatibility passed" << std::endl;
}

void runRendererTests()
{
    // displayFadingWhite();
    // displayMovingCircle();
    // displayLine();
    testRendererCompatibility();
}