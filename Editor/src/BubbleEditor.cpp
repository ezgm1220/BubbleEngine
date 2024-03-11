#include <Bubble.h>
#include <Bubble/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Bubble
{
    class Sandbox : public Application
    {
    public:
        Sandbox() : Application("Bubble Editor") { PushLayer(new EditorLayer()); }
        ~Sandbox() {}
    };

    Application* CreatApplication() { return new Sandbox(); }
}
