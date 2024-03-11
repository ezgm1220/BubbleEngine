#include "bubblepch.h"
#include "Bubble/Core/Application.h"
#include "Bubble/Core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Bubble
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name)
    {
        s_Instance = this;

        m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(BB_BIND_EVENT_FN(Application::OnEvent));// 设置回调函数

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }


    Application::~Application() {}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BB_BIND_EVENT_FN(Application::OnWindowClose));

        for(auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if(e.Handled)
                break;
            (*it)->OnEvent(e);
        }

        //BB_CORE_TRACE("{0}", e);
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::Run()
    {
        while(m_Running)
        {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            

            for(Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();

            for(Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
            m_Window->OnUpdate();

            glClearColor(227.0f / 256.0f, 180.0f / 256.0f, 184.0f / 256.0f, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        // while (true);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return false;
    }
    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        return false;
    }
} // namespace Bubble