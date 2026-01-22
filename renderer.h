#include <vector>

class Renderer
{
private:
    int width;
    int height;
    int lowerX;
    int lowerY;
    int upperX;
    int upperY;
    std::vector<int> objects;

public:
    Renderer(int w, int h, int lwx, int lwy, int upx, int upy);
    void clearScreen();
    void drawObjects();
    void render();
    void addObject(int o);
};

Renderer::Renderer(int w, int h, int lwx, int lwy, int upx, int upy)
{
    this->width = w;
    this->height = h;
    this->lowerX = lwx;
    this->lowerY = lwy;
    this->upperX = upx;
    this->upperY = upy;
}

void Renderer::clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
}

void Renderer::drawObjects()
{
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        std::cout << "drawing object " << i << std::endl;
    }
    this->objects.resize(0);
}

void Renderer::render()
{
    this->clearScreen();
    this->drawObjects();
}

void Renderer::addObject(int o)
{
    this->objects.push_back(o);
}
