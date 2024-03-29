﻿#pragma once

#include "Bubble/Core/Base.h"

#include "Bubble/Core/Window.h"
#include "Bubble/Core/LayerStack.h"
#include "Bubble/Events/Event.h"
#include "Bubble/Events/ApplicationEvent.h"

#include "Bubble/Core/Timestep.h"

#include "Bubble/ImGui/ImGuiLayer.h"

#include "Bubble/Pipeline/Pipeline.h"

int main(int argc, char** argv);

namespace Bubble
{
    class Application
    {
    public:
        Application(const std::string& name = "Bubble App");
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        void Close();

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        Window& GetWindow() { return *m_Window; }

        static Application& Get() { return *s_Instance; }



    private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    protected:
        Ref<Pipeline>m_Pipeline;
    };
    Application* CreatApplication();

} // namespace Bubble