#include <vector>
#include <cstring>

// sets all values of array to specified val (n is array size)
template <typename T>
void defval(std::vector<T> &p, T val, int n)
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

public:
    RenderObject(int w, int h) : width(w), height(h)
    {
        data.resize(width * height);
        rendist.resize(width * height);
        defval(data, 0, w * h);
        defval(rendist, -1.0, w * h);
    }

    void set(int x, int y, int d, double rndt)
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

    bool checkCompatibility(int w, int h)
    {
        return w == width and h == height;
    }

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
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                const double frac = ((x2 - x1) * (i + 0.5 - x1) + (y2 - y1) * (j + 0.5 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
                if (frac < 0 or frac > 1)
                    continue;
                const double closestx = x1 + frac * (x2 - x1);
                const double closesty = y1 + frac * (y2 - y1);
                const double dist2 = (closestx - i - 0.5) * (closestx - i - 0.5) + (closesty - j - 0.5) * (closesty - j - 0.5);

                if (dist2 <= 0.25)
                    line.set(i, j, 3, rndt1 + frac * (rndt2 - rndt1));
                else if (dist2 <= 0.5)
                    line.set(i, j, 4, rndt1 + frac * (rndt2 - rndt1));
            }
        }
        return line;
    }
};

class Renderer
{
private:
    int width;
    int height;
    std::vector<RenderObject *> buffer;
    std::vector<int> data;
    std::vector<double> rendist;

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
    void drawPixel(const int d)
    {
        if (d == 4) // half line
            std::cout << "--";
        else if (d == 3) // line
            std::cout << "--";
        else if (d == 2) // half solid
            std::cout << "tt";
        else if (d == 1) // solid
            std::cout << "##";
        else // empty
            std::cout << "  ";
    }
    void drawObjects()
    {
        defval(data, 0, width * height);
        defval(rendist, -1.0, width * height);
        for (size_t i = 0; i < this->buffer.size(); i++)
            addObjectToData(buffer[i]);
        this->buffer.resize(0);
    }
    void render()
    {
        // load the pixels
        this->clearScreen();
        this->drawObjects();

        // draw the pixels
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
                drawPixel((rendist[width * j + i] < 0) ? 0 : data[width * j + i]);
            std::cout << "\n";
        }
    }
    void addObjectToBuffer(RenderObject *o)
    {
        if (!(o->checkCompatibility(width, height)))
            throw std::invalid_argument("Object is not compatible with renderer");
        this->buffer.push_back(o);
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
};
