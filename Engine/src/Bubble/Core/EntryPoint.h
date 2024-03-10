#pragma once

extern Bubble::Application* Bubble::CreatApplication();

int main(int argc, char** argv)
{
    Bubble::Log::Init();
    auto app = Bubble::CreatApplication();
    app->Run();
    delete app;
}
