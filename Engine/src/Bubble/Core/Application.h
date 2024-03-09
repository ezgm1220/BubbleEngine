#pragma once

#include <cstdio>

namespace Bubble
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();
    };
    Application* CreatApplication();

} // namespace Bubble