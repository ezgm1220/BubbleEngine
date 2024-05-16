#pragma once
#include "Bubble/Core/Base.h"
extern Bubble::Application* Bubble::CreatApplication();

int main(int argc, char** argv)
{
    Bubble::Log::Init();
    BB_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
    auto app = Bubble::CreatApplication();
    BB_PROFILE_END_SESSION();

    BB_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->Run();
    BB_PROFILE_END_SESSION();

    BB_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
    delete app;
    BB_PROFILE_END_SESSION();
}
