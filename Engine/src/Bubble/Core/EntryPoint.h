#pragma once

extern Bubble::Application* Bubble::CreatApplication();

int main(int argc, char** argv)
{
    auto app = Bubble::CreatApplication();
    app->Run();
    delete app;
}
