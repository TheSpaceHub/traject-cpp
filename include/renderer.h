#pragma once

#include <vector>
#include <cstring>
#include <cmath>

// sets all values of array to specified val (n is array size)
template <typename T>
inline void defval(std::vector<T> &p, const T val, const int n)
{
    for (int i = 0; i < n; i++)
        p[i] = val;
}

class RenderObject
{
private:
    int width;
    int height;
    std::vector<int> data;
    std::vector<double> rendist;

    void addLine(double x1, double y1, double x2, double y2, double rndt1, double rndt2)
    {
        // adds a line with endpoints (x1, y1) and (x2, y2)
        // rendist varying from rndt1 to rndt2
        // x1, x2 € [0, w], y1, y2 € [0, h]
        // we treat a pixel's location as its midpoint
        if (x1 == x2 and y1 == y2)
            return;
        for (int j = std::max(0, (int)std::floor(std::min(y1, y2)) - 1); j < std::min(height, (int)std::ceil(std::max(y1, y2)) + 1); j++)
        {
            for (int i = std::max(0, (int)std::floor(std::min(x1, x2)) - 1); i < std::min(width, (int)std::ceil(std::max(x1, x2)) + 1); i++)
            {
                const double frac = ((x2 - x1) * (i + 0.5 - x1) + (y2 - y1) * (j + 0.5 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
                if (frac < 0 or frac > 1)
                    continue;
                const double closestx = x1 + frac * (x2 - x1);
                const double closesty = y1 + frac * (y2 - y1);
                const double dist2 = (closestx - i - 0.5) * (closestx - i - 0.5) + (closesty - j - 0.5) * (closesty - j - 0.5);

                if (rendist[j * width + i] < 0 || rndt1 + frac * (rndt2 - rndt1) <= rendist[j * width + i])
                {
                    if (dist2 <= 0.25)
                    {
                        data[j * width + i] = 3;
                        rendist[j * width + i] = rndt1 + frac * (rndt2 - rndt1);
                    }
                    else if (dist2 <= 0.75)
                    {
                        data[j * width + i] = 4;
                        rendist[j * width + i] = rndt1 + frac * (rndt2 - rndt1);
                    }
                }
            }
        }
    }

public:
    RenderObject(int w, int h) : width(w), height(h)
    {
        data.resize(width * height);
        rendist.resize(width * height);
        defval(data, 0, w * h);
        defval(rendist, -1.0, w * h);
    }

    void set(const int x, const int y, const int d, const double rndt)
    {
        // sets pixel (x, y) to data d and distance rndt
        data[y * width + x] = d;
        rendist[y * width + x] = rndt;
    }

    int getData(const int x, const int y) const
    {
        return data[width * y + x];
    }

    double getRendist(const int x, const int y) const
    {
        return rendist[width * y + x];
    }

    bool checkCompatibility(const int w, const int h) const
    {
        return w == width and h == height;
    }

    // merges passed object to called object. Does not modify the first one but it does the second one
    void merge(RenderObject *o)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (o->rendist[width * y + x] < 0)
                    continue;
                if (rendist[width * y + x] < 0 or o->rendist[width * y + x] < rendist[width * y + x])
                {
                    data[width * y + x] = o->data[width * y + x];
                    rendist[width * y + x] = o->rendist[width * y + x];
                }
            }
        }
    }

    // basic objects

    static RenderObject Circle(int w, int h, double x, double y, double r, double rndt)
    {
        // creates a circle for a renderer of size wxh with center(x, y), radius r and rendist rndt
        // x € [0, w], y € [0, h]
        // we treat a pixel's location as its midpoint
        RenderObject circle(w, h);
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                if ((x - i - 0.5) * (x - i - 0.5) + (y - j - 0.5) * (y - j - 0.5) <= (r - 0.5) * (r - 0.5))
                    circle.set(i, j, 1, rndt);
                else if ((x - i - 0.5) * (x - i - 0.5) + (y - j - 0.5) * (y - j - 0.5) <= (r + 0.5) * (r + 0.5))
                    circle.set(i, j, 2, rndt);
            }
        }
        return circle;
    }

    static RenderObject Line(int w, int h, double x1, double y1, double x2, double y2, double rndt1, double rndt2)
    {
        // creates a line for a renderer of size wxh with endpoints (x1, y1) and (x2, y2)
        // rendist varying from rndt1 to rndt2
        // x1, x2 € [0, w], y1, y2 € [0, h]
        // we treat a pixel's location as its midpoint
        RenderObject line(w, h);
        line.addLine(x1, y1, x2, y2, rndt1, rndt2);
        return line;
    }

    static RenderObject Multiline(int w, int h, const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &rndt)
    {
        // does exactly what it says
        if (x.size() != y.size() or y.size() != rndt.size())
            throw std::invalid_argument("Point and rendist data does not match");

        RenderObject o(w, h);
        for (size_t i = 1; i < x.size(); i++)
            o.addLine(x[i - 1], y[i - 1], x[i], y[i], rndt[i - 1], rndt[i]);
        return o;
    }

    static RenderObject Sphere(int w, int h, double x, double y, double r, double rndt)
    {
        // creates a sphere for a renderer of size wxh with center(x, y), radius r and radius rendist rndt
        // x € [0, w], y € [0, h]
        // we treat a pixel's location as its midpoint
        RenderObject sphere(w, h);
        if (r > rndt)
            throw std::invalid_argument("Sphere collides with screen");

        for (int j = std::max(0, (int)std::floor(y - r) - 1); j < std::min(h, (int)std::ceil(y + r) + 1); j++)
        {
            for (int i = std::max(0, (int)std::floor(x - r) - 1); i < std::min(w, (int)std::ceil(x + r) + 1); i++)
            {
                double d2 = (x - i - 0.5) * (x - i - 0.5) + (y - j - 0.5) * (y - j - 0.5);

                if (3 * d2 <= r * r)
                    sphere.set(i, j, 1, rndt - std::sqrt(r * r - d2));
                else if (r * r >= d2 * 1.25)
                    sphere.set(i, j, 5, rndt - std::sqrt(r * r - d2));
                else if ((r + 0.5) * (r + 0.5) >= d2)
                    sphere.set(i, j, 2, rndt - std::sqrt(r * r - d2));
            }
        }
        return sphere;
    }
};

class Renderer
{
private:
    int width;
    int height;
    // we use a string buffer to make display smoother
    std::string stringBuffer;
    std::vector<RenderObject *> buffer;
    std::vector<int> data;
    std::vector<double> rendist;

    void drawPixel(const int d)
    {
        if (d == 5) // 75% solid
            stringBuffer += "xx";
        else if (d == 4) // half line
            stringBuffer += "..";
        else if (d == 3) // line
            stringBuffer += "**";
        else if (d == 2) // half solid
            stringBuffer += "::";
        else if (d == 1) // solid
            stringBuffer += "##";
        else // empty
            stringBuffer += "  ";
    }

    void addObjectToData(RenderObject *o)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (o->getRendist(x, y) < 0)
                    continue;
                if (rendist[width * y + x] < 0)
                {
                    data[width * y + x] = o->getData(x, y);
                    rendist[width * y + x] = o->getRendist(x, y);
                }
                else if (o->getRendist(x, y) < rendist[width * y + x])
                {
                    data[width * y + x] = o->getData(x, y);
                    rendist[width * y + x] = o->getRendist(x, y);
                }
            }
        }
    }

public:
    Renderer(int w, int h)
    {
        width = w;
        height = h;
        data.resize(width * height);
        rendist.resize(width * height);
    }
    void clearScreen()
    {
        std::cout << "\033[2J\033[1;1H";
    }
    void drawObjects()
    {
        defval(data, 0, width * height);
        defval(rendist, -1.0, width * height);
        for (size_t i = 0; i < this->buffer.size(); i++)
            addObjectToData(buffer[i]);
        this->buffer.resize(0);
    }
    void render(bool clearScreenBeforeRendering = true)
    {
        // load the pixels
        if (clearScreenBeforeRendering)
            this->clearScreen();
        this->drawObjects();

        // reserve
        stringBuffer.reserve(width * height * 2 + height);

        // draw the pixels
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
                drawPixel((rendist[width * j + i] < 0) ? 0 : data[width * j + i]);
            stringBuffer += "\n";
        }
        std::cout << stringBuffer;
        stringBuffer = "";
    }
    void addObjectToBuffer(RenderObject *o)
    {
        if (!(o->checkCompatibility(width, height)))
            throw std::invalid_argument("Object is not compatible with renderer");
        this->buffer.push_back(o);
    }
};
