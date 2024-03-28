#include "bubblepch.h"
#include "Bubble/Core/Application.h"
#include "Bubble/Core/Log.h"

#include "Bubble/Renderer/Renderer.h"
#include "Bubble/Core/Input.h"

#include "Bubble/Pipeline/PBR_Pipeline.h"

#include <GLFW/glfw3.h>

namespace Bubble
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name)
    {

        BB_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(BB_BIND_EVENT_FN(Application::OnEvent));// 设置回调函数

        // 初始化Pipeline(默认Pipeline为PBR)
        m_Pipeline = CreateRef<PBRPipeline>();

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();

        PushOverlay(m_ImGuiLayer);
    }


    Application::~Application() {
        Renderer::Shutdown();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(BB_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BB_BIND_EVENT_FN(Application::OnWindowResize));

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

            if(!m_Minimized)
            {
                // 逻辑
                for(Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(timestep);
                }

                // UI
                m_ImGuiLayer->Begin();
                for(Layer* layer : m_LayerStack)
                {
                    layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }
   
            // 更新窗口,交换缓冲区
            m_Window->OnUpdate();
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
        if(e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
} // namespace Bubble