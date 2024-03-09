#include <Bubble.h>

class Sandbox : public Bubble::Application
{
public:
    Sandbox() {}
    ~Sandbox() {}
};

Bubble::Application* Bubble::CreatApplication() { return new Sandbox(); }