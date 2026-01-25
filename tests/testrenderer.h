#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <windows.h>
#include <cmath>
#include "../renderer.h"

#define pi 3.141592653589793238

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

void displayLineThroughCircle()
{
    RenderObject *v = new RenderObject(RenderObject::Circle(30, 30, 10, 5, 7, 5));
    RenderObject *o = new RenderObject(RenderObject::Line(30, 30, 3, 4, 20, 10, 0, 10));
    Renderer renderer(30, 30);
    renderer.addObjectToBuffer(o);
    renderer.addObjectToBuffer(v);
    renderer.render();
    delete o;
}

void displayManualCircumference()
{
    std::vector<double> x(0), y(0), rndt(0);
    int lineCount = 100;
    double r = 12;
    double x_off = 15;
    double y_off = 15;
    for (int i = 0; i <= lineCount; i++)
    {
        x.push_back(x_off + r * std::cos(2.0 * pi / (double)lineCount * (double)i));
        y.push_back(y_off + r * std::sin(2.0 * pi / (double)lineCount * (double)i));
        rndt.push_back(r + r * std::cos(2.0 * pi / (double)lineCount * (double)i));
    }

    RenderObject *circ = new RenderObject(RenderObject::Multiline(30, 30, x, y, rndt));

    Renderer renderer(30, 30);
    renderer.addObjectToBuffer(circ);
    renderer.render();
    delete circ;
}

void displayMovingLineThroughCircle()
{
    Renderer renderer = Renderer(50, 30);
    RenderObject *v = new RenderObject(RenderObject::Circle(50, 30, 15, 10, 9, 5));
    RenderObject *w = new RenderObject(RenderObject::Line(50, 30, 3, 4, 30, 20, 0, 10));
    int c = 50;
    for (int i = 0; i < 2 * c; i++)
    {
        (*w) = RenderObject::Line(50, 30, 3, 4 + 5 * std::sin(2 * pi * i / c), 30, 20 + 5 * std::sin(2 * pi * i / c), 0, 10);
        renderer.addObjectToBuffer(w);
        renderer.addObjectToBuffer(v);
        renderer.render();
        Sleep(10);
    }
    delete w;
    delete v;
}

void displaySphere()
{
    Renderer renderer = Renderer(50, 30);
    RenderObject *v = new RenderObject(RenderObject::Sphere(50, 30, 15, 10, 9, 30));
    renderer.addObjectToBuffer(v);
    renderer.render();
    delete v;
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
    testRendererCompatibility();
    // displayFadingWhite();
    displayMovingCircle();
    displayLineThroughCircle();
    displayManualCircumference();
    displayMovingLineThroughCircle();
    displaySphere();
}